/*
 * Copyright 2014  Martin Klapetek <mklapetek@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "kaccounts-ui-provider.h"
#include <KCMTelepathyAccounts/ParameterEditModel>
#include <KCMTelepathyAccounts/account-edit-widget.h>

#include <KAccounts/getcredentialsjob.h>
#include <KAccounts/core.h>

#include <SignOn/Identity>
#include <SignOn/IdentityInfo>
#include <Accounts/Account>
#include <Accounts/AccountService>
#include <Accounts/Manager>

#include <TelepathyQt/Profile>
#include <TelepathyQt/ConnectionManager>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/PendingAccount>
#include <TelepathyQt/ProfileManager>
#include <TelepathyQt/PendingStringList>
#include <TelepathyQt/AccountSet>

#include <KLocalizedString>
#include <KSharedConfig>
#include <KConfigGroup>

#include <QDBusConnection>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class KAccountsUiProvider::Private
{
public:
    Tp::ConnectionManagerPtr connectionManager;
    Tp::ProfilePtr profile;

    AccountEditWidget *accountEditWidget;
    Tp::AccountManagerPtr accountManager;
    Tp::ProfileManagerPtr profileManager;
    QDialog *dialog;
    bool thingsReady;
    QString providerName;
    KAccountsUiPlugin::UiType type;
    Tp::AccountPtr account;
    bool reconnectRequired;
    QString manager;
    QString protocol;
};

KAccountsUiProvider::KAccountsUiProvider(QObject *parent)
    : KAccountsUiPlugin(parent),
      d(new Private)
{
    d->accountEditWidget = 0;
    d->reconnectRequired = false;

    Tp::registerTypes();
}

KAccountsUiProvider::~KAccountsUiProvider()
{
    // tp managers are automatically ref-count-deleted
    delete d;
}

QStringList KAccountsUiProvider::supportedServicesForConfig() const
{
    return QStringList() << QStringLiteral("IM");
}

void KAccountsUiProvider::init(KAccountsUiPlugin::UiType type)
{
    d->type = type;

    if (d->type == KAccountsUiPlugin::ConfigureAccountDialog) {
        if (d->accountManager->isReady()) {
            Q_EMIT uiReady();
        } else {
            Tp::AccountFactoryPtr  accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                               Tp::Features() << Tp::Account::FeatureCore
                                                                               << Tp::Account::FeatureCapabilities
                                                                               << Tp::Account::FeatureProtocolInfo
                                                                               << Tp::Account::FeatureProfile);
            d->accountManager = Tp::AccountManager::create(accountFactory);
            // let's wait for AM to become ready first
            connect(d->accountManager->becomeReady(), &Tp::PendingOperation::finished, this, &KAccountsUiProvider::uiReady);
        }
    }
}

void KAccountsUiProvider::setProviderName(const QString &providerName)
{
    d->providerName = providerName;

    Accounts::ServiceList list = KAccounts::accountsManager()->serviceList(QStringLiteral("IM"));
    Accounts::Service neededService;
    Q_FOREACH (const Accounts::Service &service, list) {
        if (service.provider() == providerName) {
            neededService = service;
            break;
        }
    }

    if (!neededService.isValid()) {
        Q_EMIT  error(i18n("Cannot find the needed service file for %1. Please check your installation.", providerName));
        return;
    }

    auto nodeList = neededService.domDocument().elementsByTagName(QStringLiteral("group"));

    for (int i = 0; i < nodeList.size(); i++) {
        auto attrsMap = nodeList.at(i).attributes();
        if (attrsMap.contains(QStringLiteral("name")) && attrsMap.namedItem(QStringLiteral("name")).nodeValue() == QLatin1String("telepathy")) {
            QDomElement settingNode = nodeList.at(i).firstChildElement(QStringLiteral("setting"));

            while (!settingNode.isNull()) {
                if (settingNode.attribute(QStringLiteral("name")) == QLatin1String("manager")) {
                    d->manager = settingNode.text();
                }
                if (settingNode.attribute(QStringLiteral("name")) == QLatin1String("protocol")) {
                    d->protocol = settingNode.text();
                }

                settingNode = settingNode.nextSiblingElement(QStringLiteral("setting"));
            }

            if (!d->manager.isEmpty() && !d->protocol.isEmpty()) {
                break;
            }
        }
    }

    qDebug() << "Requested UI for manager" << d->manager << "and protocol" << d->protocol;

    if (d->profileManager.isNull()) {
        d->profileManager = Tp::ProfileManager::create(QDBusConnection::sessionBus());
    }

    Tp::PendingOperation *op = d->profileManager->becomeReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles);
    connect(op, SIGNAL(finished(Tp::PendingOperation*)), this, SLOT(onProfileManagerReady(Tp::PendingOperation*)));
}

void KAccountsUiProvider::onProfileManagerReady(Tp::PendingOperation *op)
{
    if (op && op->isError()) {
        qWarning() << "Profile manager failed to get ready:" << op->errorMessage();
        return;
    }

    // If this was called after profile manager became ready and profile name is not yet known
    // OR if profile name was set and profile manager is not yet ready, return.
    // If profile name is set and this returns, it will get through this again when profile manager
    // becomes ready and vice-versa.
    if (d->profileManager && !d->profileManager->isReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles)) {
        return;
    }

    qDebug() << "Creating service for" << d->providerName;

    // KDE Talk is a bit special case so special handling needs to be added
    bool kdeTalk = d->providerName.contains(QStringLiteral("kde-talk"));

    auto profiles = d->profileManager->profilesForProtocol(d->protocol);
    Q_FOREACH (const Tp::ProfilePtr &profile, profiles) {
        if (profile->cmName() == d->manager && (kdeTalk && profile->serviceName() == QLatin1String("kde-talk"))) {
            d->profile = profile;
            break;
        }
    }

    if (d->profile.isNull()) {
        Q_EMIT  error(i18n("This IM Account cannot be created - a Telepathy Connection Manager named '%1' is missing. Please try installing it with your package manager.", d->manager));
        return;
    }

    d->connectionManager = Tp::ConnectionManager::create(d->manager);
    Tp::PendingOperation *cmReady = d->connectionManager->becomeReady(Tp::Features() << Tp::ConnectionManager::FeatureCore);
    connect(cmReady, SIGNAL(finished(Tp::PendingOperation*)), this, SLOT(onConnectionManagerReady(Tp::PendingOperation*)));
}

void KAccountsUiProvider::onConnectionManagerReady(Tp::PendingOperation*)
{
    Tp::ProtocolInfo protocolInfo = d->connectionManager->protocol(d->profile->protocolName());
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();

    d->dialog = new QDialog();
    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(d->dialog);
    parameterModel->addItems(parameters, d->profile->parameters());

    d->dialog->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *mainLayout = new QVBoxLayout(d->dialog);
    d->dialog->setLayout(mainLayout);

    // Set up the account edit widget.
    d->accountEditWidget = new AccountEditWidget(d->profile,
                                                 QString(),
                                                 parameterModel,
                                                 doConnectOnAdd,
                                                 d->dialog);

    QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, d->dialog);
    connect(dbb, &QDialogButtonBox::accepted, this, &KAccountsUiProvider::onCreateAccountDialogAccepted);
    connect(dbb, &QDialogButtonBox::rejected, this, &KAccountsUiProvider::onCreateAccountDialogRejected);

    mainLayout->addWidget(d->accountEditWidget);
    mainLayout->addWidget(dbb);

    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)),
            d->accountEditWidget,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)));

    Q_EMIT uiReady();
}

void KAccountsUiProvider::showNewAccountDialog()
{
    d->dialog->exec();
}

void KAccountsUiProvider::showConfigureAccountDialog(const quint32 accountId)
{
    Q_FOREACH (const Tp::AccountPtr &account, d->accountManager->validAccounts()->accounts())  {
        if (account->uniqueIdentifier().endsWith(QString::number(accountId))) {
            d->account = account;
        }
    }

    if (!d->account) {
        qWarning() << "Couldn't find account for" << accountId;
        return;
    }

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->account->protocolInfo();
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();
    QVariantMap parameterValues = d->account->parameters();

    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(this);
    parameterModel->addItems(parameters, d->account->profile()->parameters(), parameterValues);

    //update the parameter model with the password from kwallet (if applicable)
    Tp::ProtocolParameter passwordParameter = parameterModel->parameter(QLatin1String("password"));


    d->dialog = new QDialog();
    d->dialog->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *mainLayout = new QVBoxLayout(d->dialog);
    d->dialog->setLayout(mainLayout);
    d->dialog->setProperty("accountId", accountId);

    QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, d->dialog);
    connect(dbb, &QDialogButtonBox::accepted, this, &KAccountsUiProvider::onConfigureAccountDialogAccepted);
    connect(dbb, &QDialogButtonBox::rejected, this, &KAccountsUiProvider::onConfigureAccountDialogRejected);

    if (passwordParameter.isValid()) {
        QModelIndex index = parameterModel->indexForParameter(passwordParameter);
        GetCredentialsJob *credentialsJob = new GetCredentialsJob(accountId, this);
        connect(credentialsJob, &GetCredentialsJob::finished, [=](KJob *job){
            QString secret = qobject_cast<GetCredentialsJob*>(job)->credentialsData().value(QLatin1String("Secret")).toString();
            parameterModel->setData(index, secret, Qt::EditRole);
            job->deleteLater();
        });
        credentialsJob->start();
    }


    d->accountEditWidget = new AccountEditWidget(d->account->profile(),
                                                 d->account->displayName(),
                                                 parameterModel,
                                                 doNotConnectOnAdd,
                                                 d->dialog);

    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)),
            d->accountEditWidget,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)), Qt::UniqueConnection);

    mainLayout->addWidget(d->accountEditWidget);
    mainLayout->addWidget(dbb);

    d->dialog->exec();
}

void KAccountsUiProvider::onCreateAccountDialogAccepted()
{
    // Get the parameter values.
    QVariantMap values  = d->accountEditWidget->parametersSet();

    // Check all pages of parameters pass validation.
    if (!d->accountEditWidget->validateParameterValues()) {
        qDebug() << "A widget failed parameter validation. Not accepting wizard.";
        Q_EMIT feedbackMessage(i18n("Failed to create account"),
                                d->accountEditWidget->errorMessage(),
                                KMessageWidget::Error);
        return;
    }

    // FIXME: In some next version of tp-qt4 there should be a convenience class for this
    // https://bugs.freedesktop.org/show_bug.cgi?id=33153
    QVariantMap properties;

    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Service"))) {
        properties.insert(QLatin1String("org.freedesktop.Telepathy.Account.Service"), d->profile->serviceName());
    }
    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"))) {
        properties.insert(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"), true);
    }

    //remove password values from being sent. These are stored by Accounts SSO instead

    //FIXME: This is a hack for jabber registration, we don't remove passwords - see Telepathy ML thread "Storing passwords in MC and regsitering new accounts"
    //http://lists.freedesktop.org/archives/telepathy/2011-September/005747.html
    if (!values.contains(QLatin1String("register"))) {
        values.remove(QLatin1String("password"));
    }

    //TODO: prefix all the values with telepathy or some mc-key
    Q_EMIT success(values[QStringLiteral("account")].toString(), values[QStringLiteral("password")].toString(), values);

    d->dialog->accept();
}

void KAccountsUiProvider::onCreateAccountDialogRejected()
{
    Q_EMIT error(QString());
    d->dialog->reject();
}

void KAccountsUiProvider::onConfigureAccountDialogAccepted()
{
    QVariantMap setParameters = d->accountEditWidget->parametersSet();
    QStringList unsetParameters = d->accountEditWidget->parametersUnset();

    // Check all pages of parameters pass validation.
    if (!d->accountEditWidget->validateParameterValues()) {
        qWarning() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    //remove password from setParameters as this is now stored by kwallet instead
    setParameters.remove(QStringLiteral("password"));
    unsetParameters.append(QStringLiteral("password")); //remove the password from mission control

    Tp::PendingStringList *psl = d->account->updateParameters(setParameters, unsetParameters);

    connect(psl, &Tp::PendingOperation::finished, this, [=](Tp::PendingOperation *op){
        if (op->isError()) {
            // FIXME: Visual feedback in GUI to user.
            qWarning() << "Could not update parameters:" << op->errorName() << op->errorMessage();
            return;
        }

        Tp::PendingStringList *psl = qobject_cast<Tp::PendingStringList*>(op);

        Q_ASSERT(psl);
        if (!psl) {
            qWarning() << "Something  weird happened; couldn't update the parameters";
        }

        if (psl->result().size() > 0) {
            qDebug() << "The following parameters won't be updated until reconnection: " << psl->result();
            d->reconnectRequired = true;
        }

        QVariantMap values = d->accountEditWidget->parametersSet();

        if (values.contains(QLatin1String("password"))) {
            //TODO Store the new password in sso
            quint32 accountId = d->dialog->property("accountId").toUInt();
            storePasswordInSso(accountId, values.value(QStringLiteral("password")).toString());
        } else {
            //TODO ...or remove it?
        }

        if (d->accountEditWidget->updateDisplayName()) {
            connect(d->account->setDisplayName(d->accountEditWidget->displayName()), &Tp::PendingOperation::finished,
                    this, [=](Tp::PendingOperation *op) {
                        if (op->isError()) {
                            qWarning() << "Error updating display name:" << op->errorName() << op->errorMessage();
                        }

                        onConfigureAccountFinished();
                    });
        } else {
            onConfigureAccountFinished();
        }
    });
}

void KAccountsUiProvider::onConfigureAccountFinished()
{
    if (d->reconnectRequired) {
        d->account->reconnect();
    }

    d->dialog->accept();
}

void KAccountsUiProvider::onConfigureAccountDialogRejected()
{
    Q_EMIT error(QString());
    d->dialog->reject();
}

void KAccountsUiProvider::storePasswordInSso(const quint32 accountId, const QString &password)
{
    Accounts::Manager *manager = KAccounts::accountsManager();
    Accounts::Account *account = manager->account(accountId);
    SignOn::Identity *identity;

    if (account) {
        Accounts::AccountService *service = new Accounts::AccountService(account, manager->service(QString()), this);
        Accounts::AuthData authData = service->authData();
        identity = SignOn::Identity::existingIdentity(authData.credentialsId(), this);

        SignOn::IdentityInfo info;
        info.setSecret(password);
        identity->storeCredentials(info);
    }
}
