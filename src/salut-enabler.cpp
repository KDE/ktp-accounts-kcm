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

#include <QFrame>

#include "salut-details-dialog.h"
#include "salut-message-widget.h"

#define TP_PROP_ACCOUNT_ENABLED (QLatin1String("org.freedesktop.Telepathy.Account.Enabled"))
#define TP_PROP_ACCOUNT_SERVICE (QLatin1String("org.freedesktop.Telepathy.Account.Service"))

class SalutEnabler::Private
{
public:
    Private(SalutEnabler* parent)
        : q(parent),
          detailsDialog(0),
          messageWidget(0)
    {
        kDebug();
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

    d->connectionManager = Tp::ConnectionManager::create("salut");
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
    kDebug();

    if(op->isError()) {
        kWarning() << "Creating ConnectionManager failed:" << op->errorName() << op->errorMessage();
    }

    if(!d->connectionManager->isValid()) {
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
    kDebug();
    if(op->isError()) {
        kWarning() << "Creating ProfileManager failed:" << op->errorName() << op->errorMessage();
    }

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->connectionManager->protocol(QLatin1String("local-xmpp"));
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();

    d->profile = d->profileManager->profilesForCM("salut").first();

    Q_ASSERT(!d->profile.isNull());
    Q_ASSERT(d->profile->isValid());
    Q_ASSERT(d->profile->protocolName() == QLatin1String("local-xmpp"));
    if (d->profile.isNull() || !d->profile->isValid() || d->profile->protocolName() != QLatin1String("local-xmpp")) {
        kWarning() << "Something went wrong with telepathy salut";
    }

    KUser user = KUser();
    QString name = user.property(KUser::FullName).toString();
    QString nick = user.loginName();
    int lastSpacePosition = name.lastIndexOf(QLatin1Char(' '));
    QString lastname = name.mid(lastSpacePosition + 1);
    QString firstName = name.left(lastSpacePosition);

    d->values.insert("first-name", firstName);
    d->values.insert("last-name", lastname);
    d->values.insert("nickname", nick);

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
    d->messageWidget->setParams(d->values["first-name"].toString(), d->values["last-name"].toString(), d->values["nickname"].toString());
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
    kDebug();


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

    QString lastname = d->values["last-name"].toString();
    QString firstname = d->values["first-name"].toString();
    QString nick = d->values["nickname"].toString();

    //either one of the names is filled and nick is filled
    if (((lastname.isEmpty() && !firstname.isEmpty()) || (!lastname.isEmpty() && firstname.isEmpty()))
            && !nick.isEmpty()) {

        displayName = QString("%1 (%2)").arg(d->values["first-name"].toString().isEmpty() ?
                                                 d->values["last-name"].toString() : d->values["first-name"].toString(),
                                             d->values["nickname"].toString());

    //either one of the names is filled and nick is empty
    } else if (((lastname.isEmpty() && !firstname.isEmpty()) || (!lastname.isEmpty() && firstname.isEmpty()))
            && nick.isEmpty()) {

        displayName = d->values["first-name"].toString().isEmpty() ?
                          d->values["last-name"].toString() : d->values["first-name"].toString();

    //both first & last names are empty but nick is not
    } else if (lastname.isEmpty() && firstname.isEmpty() && !nick.isEmpty()) {

        displayName = d->values["nickname"].toString();

    } else if (lastname.isEmpty() && firstname.isEmpty() && nick.isEmpty()) {
        //FIXME: let the user know that he reached a very strange situation

    } else {
        displayName = QString("%1 %2 (%3)").arg(d->values["first-name"].toString(),
                                                d->values["last-name"].toString(),
                                                d->values["nickname"].toString());
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
    if(op->isError()) {
        kWarning() << "Creating Account failed:" << op->errorName() << op->errorMessage();
    }

    if (op->isError()) {
        Q_EMIT feedbackMessage(i18n("Failed to create account"),
                               i18n("Possibly not all required fields are valid"),
                               KTitleWidget::ErrorMessage);
        kWarning() << "Adding Account failed:" << op->errorName() << op->errorMessage();
        return;
    }

    // Get the PendingAccount.
    Tp::PendingAccount *pendingAccount = qobject_cast<Tp::PendingAccount*>(op);
    if (!pendingAccount) {
                Q_EMIT feedbackMessage(i18n("Something went wrong with Telepathy"),
                                       QString(),
                                       KTitleWidget::ErrorMessage);
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

    connect(d->detailsDialog, SIGNAL(dialogAccepted(const QVariantMap&)),
            this, SLOT(onDialogAccepted(const QVariantMap&)));

    connect(d->detailsDialog, SIGNAL(rejected()),
            this, SLOT(onUserCancelled()));

    connect(d->detailsDialog, SIGNAL(rejected()),
            this, SIGNAL(cancelled()));

    connect(d->detailsDialog, SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)),
            this, SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)));

    d->detailsDialog->exec();
}

void SalutEnabler::onDialogAccepted(const QVariantMap &values)
{
    kDebug() << values;
    d->values.insert("first-name", values["first-name"].toString());
    d->values.insert("last-name", values["last-name"].toString());
    d->values.insert("nickname", values["nickname"].toString());

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
