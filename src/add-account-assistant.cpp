/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#include <KTp/wallet-interface.h>

#include "KCMTelepathyAccounts/abstract-account-parameters-widget.h"
#include "KCMTelepathyAccounts/abstract-account-ui.h"
#include "KCMTelepathyAccounts/account-edit-widget.h"
#include "KCMTelepathyAccounts/plugin-manager.h"
#include "KCMTelepathyAccounts/profile-item.h"
#include "KCMTelepathyAccounts/profile-list-model.h"
#include "KCMTelepathyAccounts/profile-select-widget.h"
#include "KCMTelepathyAccounts/simple-profile-select-widget.h"

#include <KDebug>
#include <KLocale>
#include <KMessageBox>
#include <KPageWidgetItem>

#include <QtCore/QList>
#include <QtGui/QHBoxLayout>
#include <QtGui/QCheckBox>

#include <TelepathyQt/PendingReady>
#include <TelepathyQt/PendingAccount>
#include <TelepathyQt/PendingOperation>

class AddAccountAssistant::Private
{
public:
    Private()
     : currentProfileItem(0),
       profileListModel(0),
       profileSelectWidget(0),
       simpleProfileSelectWidget(0),
       accountEditWidget(0),
       pageOne(0),
       pageTwo(0),
       pageThree(0)
    {
    }

    Tp::AccountManagerPtr accountManager;
    Tp::ConnectionManagerPtr connectionManager;
    ProfileItem *currentProfileItem;
    ProfileListModel *profileListModel;
    ProfileSelectWidget *profileSelectWidget;
    SimpleProfileSelectWidget *simpleProfileSelectWidget;
    AccountEditWidget *accountEditWidget;
    QWidget *pageThreeWidget;
    KPageWidgetItem *pageOne;
    KPageWidgetItem *pageTwo;
    KPageWidgetItem *pageThree;
};

AddAccountAssistant::AddAccountAssistant(Tp::AccountManagerPtr accountManager, QWidget *parent)
    : KAssistantDialog(parent),
    d(new Private)
{
    d->accountManager = accountManager;

    // Set up the pages of the Assistant.
    d->profileListModel          = new ProfileListModel(this);
    d->profileSelectWidget       = new ProfileSelectWidget(d->profileListModel, this, true);
    d->simpleProfileSelectWidget = new SimpleProfileSelectWidget(d->profileListModel, this);
    d->pageOne = new KPageWidgetItem(d->simpleProfileSelectWidget);
    d->pageTwo = new KPageWidgetItem(d->profileSelectWidget);

    d->pageOne->setHeader(i18n("Step 1: Select an Instant Messaging Network."));
    d->pageTwo->setHeader(i18n("Step 1: Select an Instant Messaging Network."));

    setValid(d->pageOne, false);
    setValid(d->pageTwo, false);

    connect(d->profileSelectWidget,
            SIGNAL(profileSelected(bool)),
            SLOT(onProfileSelected(bool)));
    connect(d->profileSelectWidget,
            SIGNAL(profileChosen()),
            SLOT(goToPageThree()));

    connect(d->simpleProfileSelectWidget,
            SIGNAL(profileChosen()),
            SLOT(goToPageThree()));
    connect(d->simpleProfileSelectWidget,
            SIGNAL(othersChosen()),
            SLOT(goToPageTwo()));

    // we will build the page widget later, but the constructor of
    // KPageWidgetItem requires the widget at this point, so...
    d->pageThreeWidget = new QWidget(this);
    new QHBoxLayout(d->pageThreeWidget);
    d->pageThree = new KPageWidgetItem(d->pageThreeWidget);
    d->pageThree->setHeader(i18n("Step 2: Fill in the required Parameters."));

    addPage(d->pageOne);
    addPage(d->pageTwo);
    addPage(d->pageThree);

    setAppropriate(d->pageTwo, false);

    // TODO re-enable the help when we will have one
    showButton(KDialog::Help, false);

    resize(QSize(400, 480));
}

AddAccountAssistant::~AddAccountAssistant()
{
    delete d;
}

void AddAccountAssistant::goToPageTwo()
{
    KAssistantDialog::setAppropriate(d->pageTwo, true);
    KAssistantDialog::next();
}

void AddAccountAssistant::goToPageThree()
{
    ProfileItem *selectedItem;

    if (currentPage() == d->pageTwo) {
        kDebug() << "Current Page seems to be page two";
        selectedItem = d->profileSelectWidget->selectedProfile();
    }
    else {
        kDebug() << "Current Page seems to be page one";
        selectedItem = d->simpleProfileSelectWidget->selectedProfile();
    }

    // FIXME: untill packages for missing profiles aren't installed this needs to stay here
    if (selectedItem != 0) {
        // Set up the next page.
        if (d->currentProfileItem != selectedItem) {
            d->currentProfileItem = selectedItem;

            d->connectionManager = Tp::ConnectionManager::create(selectedItem->cmName());
            connect(d->connectionManager->becomeReady(),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(onConnectionManagerReady(Tp::PendingOperation*)));
        }
        else {
            pageThree();
        }
    }
    else {
        KMessageBox::error(this, i18n("To connect to this IM network, you need to install additional plugins. Please install the telepathy-haze and telepathy-gabble packages using your package manager."),i18n("Missing Telepathy Connection Manager"));
    }
}

void AddAccountAssistant::next()
{
    // the next button is disabled on the first page
    // so ::next is called from the second page
    // so we go to page three now
    goToPageThree();
}

void AddAccountAssistant::back()
{
    // Disable pageTwo once we're going back to pageOne
    if (currentPage() == d->pageTwo) {
        KAssistantDialog::setAppropriate(d->pageTwo, false);
    }

    KAssistantDialog::back();
}

void AddAccountAssistant::accept()
{
    // Check we are being called from page 3.
    if (currentPage() != d->pageThree) {
        kWarning() << "Called accept() from a non-final page :(.";
        return;
    }

    // Get the parameter values.
    QVariantMap values  = d->accountEditWidget->parametersSet();

    // Check all pages of parameters pass validation.
    if (!d->accountEditWidget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        Q_EMIT feedbackMessage(i18n("Failed to create account"),
                               d->accountEditWidget->errorMessage(),
                               KMessageWidget::Error);
        return;
    }

    // FIXME: In some next version of tp-qt4 there should be a convenience class for this
    // https://bugs.freedesktop.org/show_bug.cgi?id=33153
    QVariantMap properties;

    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Service"))) {
      properties.insert(QLatin1String("org.freedesktop.Telepathy.Account.Service"), d->currentProfileItem->serviceName());
    }
    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"))) {
      properties.insert(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"), true);
    }

    // FIXME: Ask the user to submit a Display Name

    QString displayName;
    if (values.contains(QLatin1String("account"))) {
        displayName = values[QLatin1String("account")].toString();
    }
    else {
        displayName = d->currentProfileItem->protocolName();
    }

    //remove password values from being sent. These are stored by KWallet instead

    //FIXME: This is a hack for jabber registration, we don't remove passwords - see Telepathy ML thread "Storing passwords in MC and regsitering new accounts"
    //http://lists.freedesktop.org/archives/telepathy/2011-September/005747.html
    if (!values.contains(QLatin1String("register"))) {
        values.remove(QLatin1String("password"));
    }

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
    // Emit a signal to tell the assistant launcher that it was cancelled.
    Q_EMIT cancelled();

    // Close the assistant
    KAssistantDialog::reject();
}

void AddAccountAssistant::onAccountCreated(Tp::PendingOperation *op)
{
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

    Tp::AccountPtr account = pendingAccount->account();

    //save password to KWallet if needed
    QVariantMap values  = d->accountEditWidget->parametersSet();
    if (values.contains(QLatin1String("password"))) {
        KTp::WalletInterface::setPassword(account, values[QLatin1String("password")].toString());
    }

    if (d->accountEditWidget->connectOnAdd()) {
        account->setRequestedPresence(Tp::Presence::available());
    }
    account->setServiceName(d->currentProfileItem->serviceName());
    KAssistantDialog::accept();
}

void AddAccountAssistant::onConnectionManagerReady(Tp::PendingOperation *op)
{
    if (op->isError()) {
        kWarning() << "Creating ConnectionManager failed:" << op->errorName() << op->errorMessage();
    }

    if (!d->connectionManager->isValid()) {
        kWarning() << "Invalid ConnectionManager";
    }

    pageThree();
}

void AddAccountAssistant::onProfileSelected(bool value)
{
    //if a protocol is selected, enable the next button on the first page
    setValid(d->pageTwo, value);
}

void AddAccountAssistant::pageThree()
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
                                                 d->pageThreeWidget);
    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)),
            d->accountEditWidget,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)));
    d->pageThreeWidget->layout()->addWidget(d->accountEditWidget);

    KAssistantDialog::next();
}

#include "add-account-assistant.moc"
