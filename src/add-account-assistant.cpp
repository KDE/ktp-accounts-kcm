/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "KCMTelepathyAccounts/abstract-account-parameters-widget.h"
#include "KCMTelepathyAccounts/abstract-account-ui.h"
#include "KCMTelepathyAccounts/account-edit-widget.h"
#include "KCMTelepathyAccounts/connection-manager-item.h"
#include "KCMTelepathyAccounts/plugin-manager.h"
#include "KCMTelepathyAccounts/protocol-item.h"
#include "KCMTelepathyAccounts/protocol-select-widget.h"

#include <KDebug>
#include <KLocale>
#include <KMessageBox>
#include <KPageWidgetItem>

#include <QtCore/QList>
#include <QtGui/QHBoxLayout>

#include <TelepathyQt4/PendingAccount>
#include <TelepathyQt4/PendingOperation>

class AddAccountAssistant::Private
{
public:
    Private()
     : protocolSelectWidget(0),
       accountEditWidget(0),
       pageOne(0),
       pageTwo(0)
    {
        kDebug();
    }

    Tp::AccountManagerPtr accountManager;
    Tp::AccountPtr account;
    ProtocolSelectWidget *protocolSelectWidget;
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
    d->protocolSelectWidget = new ProtocolSelectWidget(this);
    d->pageOne = new KPageWidgetItem(d->protocolSelectWidget);
    d->pageOne->setHeader(i18n("Step 1: Select an Instant Messaging Network."));
    setValid(d->pageOne, false);
    connect(d->protocolSelectWidget,
            SIGNAL(protocolGotSelected(bool)),
            SLOT(onProtocolSelected(bool)));
    connect(d->protocolSelectWidget,
            SIGNAL(protocolDoubleClicked()),
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

        Q_ASSERT(d->protocolSelectWidget->selectedProtocol());

        // Set up the next page.
        ProtocolItem *item = d->protocolSelectWidget->selectedProtocol();

        ConnectionManagerItem *cmItem = qobject_cast<ConnectionManagerItem*>(item->parent());
        if (!cmItem) {
            kWarning() << "cmItem is invalid.";
        }

        // Delete the widgets for the next page if they already exist
        if (d->accountEditWidget) {
            d->accountEditWidget->deleteLater();
            d->accountEditWidget = 0;
        }

        // Set up the account edit widget
        d->accountEditWidget = new AccountEditWidget(item->protocolInfo(),
                                                     QVariantMap(),
                                                     d->pageTwoWidget);
        d->pageTwoWidget->layout()->addWidget(d->accountEditWidget);

        KAssistantDialog::next();
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

    // Check all pages of parameters pass validation.
    if (!d->accountEditWidget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    // Get the parameter values.
    QList<ProtocolParameterValue> parameterValues;
    parameterValues = d->accountEditWidget->parameterValues();

    // Get the ProtocolItem that was selected and the corresponding ConnectionManagerItem.
    ProtocolItem *protocolItem = d->protocolSelectWidget->selectedProtocol();
    ConnectionManagerItem *connectionManagerItem = qobject_cast<ConnectionManagerItem*>(protocolItem->parent());

    if (!connectionManagerItem) {
        kWarning() << "Invalid ConnectionManager item.";
        return;
    }

    QVariantMap values;
    foreach(const ProtocolParameterValue &ppv, parameterValues)
    {
        if (ppv.shouldSave()) {
            values.insert(ppv.name(), ppv.value());
        }
    }

    // FIXME: Ask the user to submit a Display Name
    Tp::PendingAccount *pa = d->accountManager->createAccount(connectionManagerItem->connectionManager()->name(),
                                                              protocolItem->protocol(),
                                                              values["account"].toString(),
                                                              values);

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
        // TODO: User feedback in this case.
        kWarning() << "Adding Account failed:" << op->errorName() << op->errorMessage();
        return;
    }

    // Get the PendingAccount.
    Tp::PendingAccount *pendingAccount = qobject_cast<Tp::PendingAccount*>(op);
    if (!pendingAccount) {
        // TODO: User visible feedback
        kWarning() << "Method called with wrong type.";
        return;
    }

    // Get the account pointer.
    d->account = pendingAccount->account();

    // Set the account icon
    QString icon = QString("im-%1").arg(d->account->protocolName());
    kDebug() << "Set account icon to: " << icon;
    d->account->setIconName(icon);

    kDebug() << "Calling set enabled.";

    connect(d->account->setEnabled(true),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onSetEnabledFinished(Tp::PendingOperation*)));
}

void AddAccountAssistant::onSetEnabledFinished(Tp::PendingOperation *op)
{
    kDebug();

    if (op->isError()) {
        // TODO: User feedback in this case.
        kWarning() << "Enabling Account failed:" << op->errorName() << op->errorMessage();
        return;
    }

    KAssistantDialog::accept();
}

void AddAccountAssistant::onProtocolSelected(bool value)
{
    kDebug();
    //if a protocol is selected, enable the next button on the first page
    setValid(d->pageOne, value);
}

#include "add-account-assistant.moc"

