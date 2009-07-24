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

#include "connection-manager-item.h"
#include "parameter-edit-widget.h"
#include "protocol-item.h"
#include "protocol-select-widget.h"

#include <KDebug>
#include <KLocale>
#include <KMessageBox>
#include <KPageWidgetItem>
#include <KTabWidget>

#include <TelepathyQt4/PendingAccount>
#include <TelepathyQt4/PendingOperation>

class AddAccountAssistant::Private
{
public:
    Private()
     : protocolSelectWidget(0),
       tabWidget(0),
       mandatoryParametersWidget(0),
       optionalParametersWidget(0),
       pageOne(0),
       pageTwo(0)
    {
        kDebug();
    }

    Tp::AccountManagerPtr accountManager;
    ProtocolSelectWidget *protocolSelectWidget;
    KTabWidget *tabWidget;
    ParameterEditWidget *mandatoryParametersWidget;
    ParameterEditWidget *optionalParametersWidget;
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
    d->tabWidget = new KTabWidget(this);
    d->pageTwo = new KPageWidgetItem(d->tabWidget);
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

void AddAccountAssistant::back()
{
    kDebug();

    // TODO: Implement me!
}

void AddAccountAssistant::next()
{
    kDebug();

    // Check which page we are on.
    if (currentPage() == d->pageOne) {
        kDebug() << "Current page: Page 1.";
        // Page 1

        // Check if the protocol is selected. If it is, set up the next page and continue to it.
        if (d->protocolSelectWidget->selectedProtocol()) {
            kDebug() << "Protocol is selected. Set up and continue to the next page.";

            // Set up the next page.
            ProtocolItem *item = d->protocolSelectWidget->selectedProtocol();

            // Delete the widgets for the next page if they already exist
            if (d->mandatoryParametersWidget) {
                d->mandatoryParametersWidget->deleteLater();
                d->mandatoryParametersWidget = 0;
            }

            if (d->optionalParametersWidget) {
                d->optionalParametersWidget->deleteLater();
                d->optionalParametersWidget = 0;
            }

            d->mandatoryParametersWidget = new ParameterEditWidget(d->tabWidget);
            d->mandatoryParametersWidget->setParameters(item->mandatoryParameters());
            d->tabWidget->addTab(d->mandatoryParametersWidget, i18n("Mandatory Parameters"));

            d->optionalParametersWidget = new ParameterEditWidget(d->tabWidget);
            d->optionalParametersWidget->setParameters(item->optionalParameters());
            d->tabWidget->addTab(d->optionalParametersWidget, i18n("Optional Parameters"));

            KAssistantDialog::next();
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

    // Get the mandatory properties.
    QMap<Tp::ProtocolParameter*, QVariant> mandatoryParameterValues =
            d->mandatoryParametersWidget->parameterValues();

    foreach (const QVariant &value, mandatoryParameterValues.values()) {
        if (value.toString().isEmpty()) {
            KMessageBox::error(this, i18n("Please enter all mandatory parameters."));
            return;
        }
    }

    // Get the optional properties
    QMap<Tp::ProtocolParameter*, QVariant> optionalParameterValues =
            d->optionalParametersWidget->parameterValues();

    // Get the ProtocolItem that was selected and the corresponding ConnectionManagerItem.
    ProtocolItem *protocolItem = d->protocolSelectWidget->selectedProtocol();
    ConnectionManagerItem *connectionManagerItem = qobject_cast<ConnectionManagerItem*>(protocolItem->parent());

    if (!connectionManagerItem) {
        kWarning() << "Invalid ConnectionManager item.";
        return;
    }

    // Merge the parameters into a QVariantMap for submitting to the Telepathy AM.
    QVariantMap parameters;

    foreach (Tp::ProtocolParameter *pp, mandatoryParameterValues.keys()) {
        QVariant value = mandatoryParameterValues.value(pp);

        // Don't try and add empty parameters.
        if (!value.isNull()) {
            parameters.insert(pp->name(), value);
        }
    }

    foreach (Tp::ProtocolParameter *pp, optionalParameterValues.keys()) {
        QVariant value = optionalParameterValues.value(pp);

        // Don't try and add empty parameters.
        if (!value.isNull()) {
            parameters.insert(pp->name(), value);
        }
    }

    // FIXME: Ask the user to submit a Display Name
    Tp::PendingAccount *pa = d->accountManager->createAccount(connectionManagerItem->connectionManager()->name(),
                                                              protocolItem->protocol(),
                                                              parameters.value("account").toString(),
                                                              parameters);

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
    if (op->isError()) {
        kWarning() << "Adding Account failed:" << op->errorName() << op->errorMessage();
        return;
    }

    KAssistantDialog::accept();
}

