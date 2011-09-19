/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "salut-enable-dialog.h"

#include "KCMTelepathyAccounts/parameter-edit-model.h"
#include "KCMTelepathyAccounts/account-edit-widget.h"
#include "KCMTelepathyAccounts/profile-item.h"

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/ConnectionManager>
#include <TelepathyQt4/ProfileManager>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingAccount>

class SalutEnableDialog::Private
{
public:
    Private(SalutEnableDialog* parent)
        : q(parent),
          widget(0)
    {
        kDebug();
    }

    SalutEnableDialog *q;
    AccountEditWidget *widget;

    Tp::ConnectionManagerPtr connectionManager;
    Tp::ProfileManagerPtr profileManager;
    Tp::AccountManagerPtr accountManager;
    Tp::ProfilePtr profile;
};

SalutEnableDialog::SalutEnableDialog(Tp::AccountManagerPtr accountManager, QWidget *parent)
    : KDialog(parent),
      d(new Private(this))
{
    kDebug();

    d->accountManager = accountManager;

    setMinimumWidth(400);

    d->connectionManager = Tp::ConnectionManager::create("salut");
    connect(d->connectionManager->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onConnectionManagerReady(Tp::PendingOperation*)));
}

SalutEnableDialog::~SalutEnableDialog()
{
    kDebug();
    delete d;
}

void SalutEnableDialog::onConnectionManagerReady(Tp::PendingOperation* op)
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

void SalutEnableDialog::onProfileManagerReady(Tp::PendingOperation* op)
{
    if(op->isError()) {
        kWarning() << "Creating ProfileManager failed:" << op->errorName() << op->errorMessage();
    }

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->connectionManager->protocol(QLatin1String("local-xmpp"));
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();

    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(this);
    d->profile = d->profileManager->profilesForCM("salut").first();
    parameterModel->addItems(parameters, d->profile->parameters());

    // Set up the interface
    d->widget = new AccountEditWidget(d->profile,
                                      parameterModel,
                                      doNotConnectOnAdd,
                                      this);
    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)),
            d->widget,
            SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)));
    setMainWidget(d->widget);

}

void SalutEnableDialog::onAccountCreated(Tp::PendingOperation* op)
{
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

    pendingAccount->account()->setRequestedPresence(Tp::Presence::available(QString("Online")));
    pendingAccount->account()->setServiceName(d->profile->serviceName());

    // set the dialog as accepted and exit
    done(KDialog::Accepted);
}

void SalutEnableDialog::accept()
{
    kDebug();

    // Get the parameter values.
    QVariantMap values = d->widget->parametersSet();

    // Check all pages of parameters pass validation.
    if (!d->widget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    // FIXME: In some next version of tp-qt4 there should be a convenience class for this
    // https://bugs.freedesktop.org/show_bug.cgi?id=33153
    QVariantMap properties;

    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Service"))) {
      properties.insert("org.freedesktop.Telepathy.Account.Service", d->profile->serviceName());
    }
    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"))) {
      properties.insert("org.freedesktop.Telepathy.Account.Enabled", true);
    }

    // FIXME: Ask the user to submit a Display Name

    QString displayName = values["first-name"].toString();
    displayName += QLatin1Char(' ');
    displayName += values["last-name"].toString();
    displayName += QLatin1Char(' ');
    if (values.contains("nickname")) {
        displayName += QLatin1Char('(');
        displayName += values["nickname"].toString();
        displayName += QLatin1Char(')');
        displayName += QLatin1Char(' ');
    }
    displayName += i18n("on local network");

    Tp::PendingAccount *pa = d->accountManager->createAccount(d->profile->cmName(),
                                                              d->profile->protocolName(),
                                                              displayName,
                                                              values,
                                                              properties);

    connect(pa,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountCreated(Tp::PendingOperation*)));
}


#include "salut-enable-dialog.moc"