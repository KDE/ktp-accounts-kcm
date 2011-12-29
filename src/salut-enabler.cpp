/*
    Class that auto configures link-local xmpp account
    Copyright (C) 2011  Martin Klapetek <martin.klapetek@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "salut-enabler.h"

#include <TelepathyQt/ConnectionManager>
#include <TelepathyQt/ProfileManager>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/PendingAccount>

#include <KDebug>
#include <KUser>
#include <KLocalizedString>

#include <QtGui/QFrame>

#include "salut-details-dialog.h"
#include "salut-message-widget.h"

#define TP_PROP_ACCOUNT_ENABLED (QLatin1String("org.freedesktop.Telepathy.Account.Enabled"))
#define TP_PROP_ACCOUNT_SERVICE (QLatin1String("org.freedesktop.Telepathy.Account.Service"))

const QLatin1String salutConnManager("salut");
const QLatin1String localXmppProtocol("local-xmpp");
const QLatin1String firstNamePar("first-name");
const QLatin1String lastNamePar("last-name");
const QLatin1String nickNamePar("nickname");

class SalutEnabler::Private
{
public:
    Private(SalutEnabler* parent)
        : q(parent),
          detailsDialog(0),
          messageWidget(0)
    {
    }

    SalutEnabler *q;

    Tp::ConnectionManagerPtr connectionManager;
    Tp::ProfileManagerPtr profileManager;
    Tp::AccountManagerPtr accountManager;
    Tp::ProfilePtr profile;
    QVariantMap values;
    SalutDetailsDialog *detailsDialog;
    SalutMessageWidget *messageWidget;
    QWeakPointer<QFrame> salutMessageFrame;
};

SalutEnabler::SalutEnabler(const Tp::AccountManagerPtr accountManager, QObject *parent)
    : QObject(parent),
      d(new Private(this))
{
    d->accountManager = accountManager;

    d->connectionManager = Tp::ConnectionManager::create(salutConnManager);
    connect(d->connectionManager->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onConnectionManagerReady(Tp::PendingOperation*)));
}

SalutEnabler::~SalutEnabler()
{
    delete d;
}

void SalutEnabler::onConnectionManagerReady(Tp::PendingOperation* op)
{
    if (op->isError()) {
        kWarning() << "Creating ConnectionManager failed:" << op->errorName() << op->errorMessage();
    }

    if (!d->connectionManager->isValid()) {
        kWarning() << "Invalid ConnectionManager";
    }

    d->profileManager = Tp::ProfileManager::create(QDBusConnection::sessionBus());

    // FIXME: Until all distros ship correct profile files, we should fake them
    connect(d->profileManager->becomeReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onProfileManagerReady(Tp::PendingOperation*)));
}

void SalutEnabler::onProfileManagerReady(Tp::PendingOperation* op)
{
    if (op->isError()) {
        kWarning() << "Creating ProfileManager failed:" << op->errorName() << op->errorMessage();
    }

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->connectionManager->protocol(localXmppProtocol);
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();

    d->profile = d->profileManager->profilesForCM(salutConnManager).first();

    Q_ASSERT(!d->profile.isNull());
    Q_ASSERT(d->profile->isValid());
    Q_ASSERT(d->profile->protocolName() == localXmppProtocol);
    if (d->profile.isNull() || !d->profile->isValid() || d->profile->protocolName() != localXmppProtocol) {
        kWarning() << "Something went wrong with telepathy salut";
    }

    KUser user = KUser();
    QString name = user.property(KUser::FullName).toString();
    QString nick = user.loginName();
    int lastSpacePosition = name.lastIndexOf(QLatin1Char(' '));
    QString lastname = name.mid(lastSpacePosition + 1);
    QString firstName = name.left(lastSpacePosition);

    d->values.insert(firstNamePar, firstName);
    d->values.insert(lastNamePar, lastname);
    d->values.insert(nickNamePar, nick);

    Q_EMIT userInfoReady();
}

QFrame* SalutEnabler::frameWidget(QWidget* parent)
{
    if (d->salutMessageFrame.isNull()) {
        d->salutMessageFrame = new QFrame(parent);
    }
    d->salutMessageFrame.data()->setMinimumWidth(parent->width());
    d->salutMessageFrame.data()->setFrameShape(QFrame::StyledPanel);

    d->messageWidget = new SalutMessageWidget(d->salutMessageFrame.data());
    d->messageWidget->setParams(d->values[firstNamePar].toString(),
                                d->values[lastNamePar].toString(),
                                d->values[nickNamePar].toString());
    d->messageWidget->hide();

    QPropertyAnimation *animation = new QPropertyAnimation(d->salutMessageFrame.data(), "minimumHeight", d->messageWidget);
    animation->setDuration(150);
    animation->setStartValue(0);
    animation->setEndValue(d->messageWidget->sizeHint().height());
    animation->start();

    connect(animation, SIGNAL(finished()),
            d->messageWidget, SLOT(animatedShow()));

    connect(d->messageWidget, SIGNAL(timeout()),
            this, SLOT(onUserAccepted()));

    connect(d->messageWidget, SIGNAL(configPressed()),
            this, SLOT(onUserWantingChanges()));

    connect(d->messageWidget, SIGNAL(cancelPressed()),
            this, SLOT(onUserCancelled()));

    return d->salutMessageFrame.data();
}

void SalutEnabler::onUserAccepted()
{
    // FIXME: In some next version of tp-qt4 there should be a convenience class for this
    // https://bugs.freedesktop.org/show_bug.cgi?id=33153
    QVariantMap properties;

    if (d->accountManager->supportedAccountProperties().contains(TP_PROP_ACCOUNT_SERVICE)) {
        properties.insert(TP_PROP_ACCOUNT_SERVICE, d->profile->serviceName());
    }
    if (d->accountManager->supportedAccountProperties().contains(TP_PROP_ACCOUNT_ENABLED)) {
        properties.insert(TP_PROP_ACCOUNT_ENABLED, true);
    }

    // FIXME: Ask the user to submit a Display Name

    QString displayName;

    QString lastname = d->values[lastNamePar].toString();
    QString firstname = d->values[firstNamePar].toString();
    QString nick = d->values[nickNamePar].toString();

    //either one of the names is filled and nick is filled
    if (((lastname.isEmpty() && !firstname.isEmpty()) || (!lastname.isEmpty() && firstname.isEmpty()))
            && !nick.isEmpty()) {

        displayName = QString::fromLatin1("%1 (%2)").arg(d->values[firstNamePar].toString().isEmpty() ?
                d->values[lastNamePar].toString() : d->values[firstNamePar].toString(),
                                                            d->values[nickNamePar].toString());

    //either one of the names is filled and nick is empty
    } else if (((lastname.isEmpty() && !firstname.isEmpty()) || (!lastname.isEmpty() && firstname.isEmpty()))
            && nick.isEmpty()) {

        displayName = d->values[firstNamePar].toString().isEmpty() ?
                d->values[lastNamePar].toString() : d->values[firstNamePar].toString();

    //both first & last names are empty but nick is not
    } else if (lastname.isEmpty() && firstname.isEmpty() && !nick.isEmpty()) {

        displayName = d->values[nickNamePar].toString();

    } else if (lastname.isEmpty() && firstname.isEmpty() && nick.isEmpty()) {
        //FIXME: let the user know that he reached a very strange situation

    } else {
        displayName = QString::fromLatin1("%1 %2 (%3)").arg(d->values[firstNamePar].toString(),
                                                               d->values[lastNamePar].toString(),
                                                               d->values[nickNamePar].toString());
    }

    Tp::PendingAccount *pa = d->accountManager->createAccount(d->profile->cmName(),
                                                              d->profile->protocolName(),
                                                              displayName,
                                                              d->values,
                                                              properties);

    connect(pa,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountCreated(Tp::PendingOperation*)));
}

void SalutEnabler::onAccountCreated(Tp::PendingOperation* op)
{
    kWarning() << "Account created";
    if (op->isError()) {
        kWarning() << "Creating Account failed:" << op->errorName() << op->errorMessage();
    }

    if (op->isError()) {
        Q_EMIT feedbackMessage(i18n("Failed to create account"),
                               i18n("Possibly not all required fields are valid"),
                               KMessageWidget::Error);
        kWarning() << "Adding Account failed:" << op->errorName() << op->errorMessage();
        return;
    }

    // Get the PendingAccount.
    Tp::PendingAccount *pendingAccount = qobject_cast<Tp::PendingAccount*>(op);
    if (!pendingAccount) {
                Q_EMIT feedbackMessage(i18n("Something went wrong with Telepathy"),
                                       QString(),
                                       KMessageWidget::Error);
        kWarning() << "Method called with wrong type.";
        return;
    }

    pendingAccount->account()->setRequestedPresence(Tp::Presence::available());
    pendingAccount->account()->setServiceName(d->profile->serviceName());

    d->salutMessageFrame.data()->deleteLater();;

    Q_EMIT done();
}

void SalutEnabler::onUserWantingChanges()
{
    d->detailsDialog = new SalutDetailsDialog(d->profileManager, d->connectionManager, 0);

    connect(d->detailsDialog, SIGNAL(dialogAccepted(QVariantMap)),
            this, SLOT(onDialogAccepted(QVariantMap)));

    connect(d->detailsDialog, SIGNAL(rejected()),
            this, SLOT(onUserCancelled()));

    connect(d->detailsDialog, SIGNAL(rejected()),
            this, SIGNAL(cancelled()));

    connect(d->detailsDialog, SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)),
            this, SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)));

    d->detailsDialog->exec();
}

void SalutEnabler::onDialogAccepted(const QVariantMap &values)
{
    kDebug() << values;
    d->values.insert(firstNamePar, values[firstNamePar].toString());
    d->values.insert(lastNamePar, values[lastNamePar].toString());
    d->values.insert(nickNamePar, values[nickNamePar].toString());

    onUserAccepted();
}

void SalutEnabler::onUserCancelled()
{
    d->messageWidget->animatedHide();

    QPropertyAnimation *animation = new QPropertyAnimation(d->salutMessageFrame.data(), "maximumHeight", d->messageWidget);
    animation->setDuration(150);
    animation->setStartValue(d->messageWidget->sizeHint().height());
    animation->setEndValue(0);

    QTimer::singleShot(300, animation, SLOT(start()));

    connect(animation, SIGNAL(finished()),
            d->salutMessageFrame.data(), SLOT(deleteLater()));

    connect(animation, SIGNAL(finished()),
            this, SIGNAL(cancelled()));
}
