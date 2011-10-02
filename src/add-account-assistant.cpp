/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
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

#include "add-account-assistant.h"

#include "wallet-interface.h"

#include "KCMTelepathyAccounts/abstract-account-parameters-widget.h"
#include "KCMTelepathyAccounts/abstract-account-ui.h"
#include "KCMTelepathyAccounts/account-edit-widget.h"
#include "KCMTelepathyAccounts/plugin-manager.h"
#include "KCMTelepathyAccounts/profile-item.h"
#include "KCMTelepathyAccounts/profile-select-widget.h"

#include <KDebug>
#include <KLocale>
#include <KMessageBox>
#include <KPageWidgetItem>
#include <KWallet/Wallet>

#include <QtCore/QList>
#include <QtGui/QHBoxLayout>
#include <QtGui/QCheckBox>

#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingAccount>
#include <TelepathyQt4/PendingOperation>

class AddAccountAssistant::Private
{
public:
    Private()
     : currentProfileItem(0),
       profileSelectWidget(0),
       accountEditWidget(0),
       pageOne(0),
       pageTwo(0)
    {
        kDebug();
    }

    Tp::AccountManagerPtr accountManager;
    Tp::ConnectionManagerPtr connectionManager;
    ProfileItem *currentProfileItem;
    ProfileSelectWidget *profileSelectWidget;
    AccountEditWidget *accountEditWidget;
    QWidget *pageTwoWidget;
    KPageWidgetItem *pageOne;
    KPageWidgetItem *pageTwo;
};

AddAccountAssistant::AddAccountAssistant(Tp::AccountManagerPtr accountManager, QWidget *parent)
 : KAssistantDialog(parent),
   d(new Private)
{
    kDebug();

    d->accountManager = accountManager;

    // Set up the pages of the Assistant.
    d->profileSelectWidget = new ProfileSelectWidget(this);
    d->pageOne = new KPageWidgetItem(d->profileSelectWidget);
    d->pageOne->setHeader(i18n("Step 1: Select an Instant Messaging Network."));
    setValid(d->pageOne, false);
    connect(d->profileSelectWidget,
            SIGNAL(profileGotSelected(bool)),
            SLOT(onProfileSelected(bool)));
    connect(d->profileSelectWidget,
            SIGNAL(profileDoubleClicked()),
            SLOT(next()));

    // we will build the page widget later, but the constructor of
    // KPageWidgetItem requires the widget at this point, so...
    d->pageTwoWidget = new QWidget(this);
    new QHBoxLayout(d->pageTwoWidget);
    d->pageTwo = new KPageWidgetItem(d->pageTwoWidget);
    d->pageTwo->setHeader(i18n("Step 2: Fill in the required Parameters."));

    addPage(d->pageOne);
    addPage(d->pageTwo);

    resize(QSize(400, 480));
}

AddAccountAssistant::~AddAccountAssistant()
{
    kDebug();

    delete d;
}

// FIXME: This method *works*, but is really not very elegant. I don't want to waste time tidying it
// up at the moment, but I'm sure it could have a *lot* less code in it if it were tidied up at some
// point in the future.
void AddAccountAssistant::next()
{
    kDebug();

    // Check which page we are on.
    if (currentPage() == d->pageOne) {
        kDebug() << "Current page: Page 1.";

        Q_ASSERT(d->profileSelectWidget->selectedProfile());

        ProfileItem *selectedItem = d->profileSelectWidget->selectedProfile();

        // Set up the next page.
        if(d->currentProfileItem != selectedItem) {
            d->currentProfileItem = selectedItem;

            d->connectionManager = Tp::ConnectionManager::create(selectedItem->cmName());
            connect(d->connectionManager->becomeReady(),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(onConnectionManagerReady(Tp::PendingOperation*)));
        }
        else {
            pageTwo();
        }
    }
}

void AddAccountAssistant::accept()
{
    kDebug();

    // Check we are being called from page 2.
    if (currentPage() != d->pageTwo) {
        kWarning() << "Called accept() from a non-final page :(.";
        return;
    }

    // Get the parameter values.
    QVariantMap values  = d->accountEditWidget->parametersSet();

    // Check all pages of parameters pass validation.
    if (!d->accountEditWidget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    // FIXME: In some next version of tp-qt4 there should be a convenience class for this
    // https://bugs.freedesktop.org/show_bug.cgi?id=33153
    QVariantMap properties;

    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Service"))) {
      properties.insert("org.freedesktop.Telepathy.Account.Service", d->currentProfileItem->serviceName());
    }
    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"))) {
      properties.insert("org.freedesktop.Telepathy.Account.Enabled", true);
    }

    // FIXME: Ask the user to submit a Display Name

    QString displayName;
    if (values.contains("account")) {
        displayName = values["account"].toString();
    }
    else {
        displayName = d->currentProfileItem->protocolName();
    }

    //remove password values from being sent. These are stored by MC instead

    //FIXME: This breaks jabber registration - see Telepathy ML thread "Storing passwords in MC and regsitering new accounts"
    values.remove("password");

    Tp::PendingAccount *pa = d->accountManager->createAccount(d->currentProfileItem->cmName(),
                                                              d->currentProfileItem->protocolName(),
                                                              displayName,
                                                              values,
                                                              properties);

    connect(pa,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountCreated(Tp::PendingOperation*)));
}

void AddAccountAssistant::reject()
{
    kDebug();

    // Emit a signal to tell the assistant launcher that it was cancelled.
    Q_EMIT cancelled();

    // Close the assistant
    KAssistantDialog::reject();
}

void AddAccountAssistant::onAccountCreated(Tp::PendingOperation *op)
{
    kDebug();

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

    Tp::AccountPtr account = pendingAccount->account();

    if(d->accountEditWidget->connectOnAdd()){
        account->setRequestedPresence(Tp::Presence::available(QString("Online")));
    }
    account->setServiceName(d->currentProfileItem->serviceName());

    //save password to KWallet if needed
    QVariantMap values  = d->accountEditWidget->parametersSet();
    if (values.contains(QLatin1String("password"))) {
        WalletInterface wallet(this->effectiveWinId());
        wallet.setPassword(account, values["password"].toString());
    }

    KAssistantDialog::accept();
}

void AddAccountAssistant::onConnectionManagerReady(Tp::PendingOperation *op)
{
    kDebug();

    if(op->isError()) {
        kWarning() << "Creating ConnectionManager failed:" << op->errorName() << op->errorMessage();
    }

    if(!d->connectionManager->isValid()) {
        kWarning() << "Invalid ConnectionManager";
    }

    pageTwo();
}

void AddAccountAssistant::onProfileSelected(bool value)
{
    kDebug();
    //if a protocol is selected, enable the next button on the first page
    setValid(d->pageOne, value);
}

void AddAccountAssistant::pageTwo()
{
    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->connectionManager->protocol(d->currentProfileItem->protocolName());
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();

    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(this);
    parameterModel->addItems(parameters, d->currentProfileItem->profile()->parameters());

    // Delete account previous widget if it already existed.
    if (d->accountEditWidget) {
        d->accountEditWidget->deleteLater();
        d->accountEditWidget = 0;
    }

    // Set up the account edit widget.
    d->accountEditWidget = new AccountEditWidget(d->currentProfileItem->profile(),
                                                 parameterModel,
                                                 doConnectOnAdd,
                                                 d->pageTwoWidget);
    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)),
            d->accountEditWidget,
            SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)));
    d->pageTwoWidget->layout()->addWidget(d->accountEditWidget);

    KAssistantDialog::next();
}

#include "add-account-assistant.moc"

