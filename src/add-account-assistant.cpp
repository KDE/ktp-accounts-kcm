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
#include "mandatory-parameter-edit-widget.h"
#include "optional-parameter-edit-widget.h"
#include "plugin-manager.h"
#include "protocol-item.h"
#include "protocol-select-widget.h"

#include "libkcmtelepathyaccounts/abstract-account-parameters-widget.h"
#include "libkcmtelepathyaccounts/abstract-account-ui.h"

#include <KDebug>
#include <KLocale>
#include <KMessageBox>
#include <KPageWidgetItem>
#include <KTabWidget>

#include <QtCore/QList>

#include <TelepathyQt4/PendingAccount>
#include <TelepathyQt4/PendingOperation>

class AddAccountAssistant::Private
{
public:
    Private()
     : protocolSelectWidget(0),
       tabWidget(0),
       mandatoryParametersWidget(0),
       pageOne(0),
       pageTwo(0)
    {
        kDebug();
    }

    Tp::AccountManagerPtr accountManager;
    ProtocolSelectWidget *protocolSelectWidget;
    KTabWidget *tabWidget;
    AbstractAccountParametersWidget *mandatoryParametersWidget;
    QList<AbstractAccountParametersWidget*> optionalParametersWidgets;
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

// FIXME: This method *works*, but is really not very elegant. I don't want to waste time tidying it
// up at the moment, but I'm sure it could have a *lot* less code in it if it were tidied up at some
// point in the future.
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

            ConnectionManagerItem *cmItem = qobject_cast<ConnectionManagerItem*>(item->parent());
            if (!cmItem) {
                kWarning() << "cmItem is invalid.";
            }

            QString connectionManager = cmItem->connectionManager()->name();
            QString protocol = item->protocol();

            // Get the AccountsUi for the plugin, and get the optional parameter widgets for it.
            AbstractAccountUi *ui = PluginManager::instance()->accountUiForProtocol(connectionManager,
                                                                                    protocol);

            // Delete the widgets for the next page if they already exist
            if (d->mandatoryParametersWidget) {
                d->mandatoryParametersWidget->deleteLater();
                d->mandatoryParametersWidget = 0;
            }

            foreach (AbstractAccountParametersWidget *w, d->optionalParametersWidgets) {
                if (w) {
                    w->deleteLater();
                }
            }
            d->optionalParametersWidgets.clear();

            // Set up the Mandatory Parameters page
            Tp::ProtocolParameterList mandatoryParameters = item->mandatoryParameters();
            Tp::ProtocolParameterList mandatoryParametersLeft = item->mandatoryParameters();

            // Create the custom UI or generic UI depending on available parameters.
            if (ui) {
                // UI does exist, set it up.
                AbstractAccountParametersWidget *widget = ui->mandatoryParametersWidget(mandatoryParameters);
                QMap<QString, QVariant::Type> manParams = ui->supportedMandatoryParameters();
                QMap<QString, QVariant::Type>::const_iterator manIter = manParams.constBegin();
                while(manIter != manParams.constEnd()) {
                    foreach (Tp::ProtocolParameter *parameter, mandatoryParameters) {
                        // If the parameter is not
                        if ((parameter->name() == manIter.key()) &&
                            (parameter->type() == manIter.value())) {
                            mandatoryParametersLeft.removeAll(parameter);
                        }
                    }

                    ++manIter;
                }

                if (mandatoryParametersLeft.isEmpty()) {
                    d->mandatoryParametersWidget = widget;
                } else {
                    // FIXME: At the moment, if the custom widget can't handle all the mandatory
                    // parameters we fall back to the generic one for all of them. It might be nicer
                    // to have the custom UI for as many as possible, and stick a small generic one
                    // underneath for those parameters it doesn't handle.
                    widget->deleteLater();
                    widget = 0;
                }
            }

            if (!d->mandatoryParametersWidget) {
                d->mandatoryParametersWidget = new MandatoryParameterEditWidget(item->mandatoryParameters(), d->tabWidget);
            }

            d->tabWidget->addTab(d->mandatoryParametersWidget, i18n("Mandatory Parameters"));

            // Get the list of parameters
            Tp::ProtocolParameterList optionalParameters = item->optionalParameters();
            Tp::ProtocolParameterList optionalParametersLeft = item->optionalParameters();

            // Check if the AbstractAccountUi exists. If not then we use the autogenerated UI for
            // everything.
            if (ui) {
                // UI Does exist, set it up.
                QList<AbstractAccountParametersWidget*> widgets = ui->optionalParametersWidgets(optionalParameters);

                // Remove all handled parameters from the optionalParameters list.
                QMap<QString, QVariant::Type> opParams = ui->supportedOptionalParameters();
                QMap<QString, QVariant::Type>::const_iterator opIter = opParams.constBegin();
                while(opIter != opParams.constEnd()) {
                    foreach (Tp::ProtocolParameter *parameter, optionalParameters) {
                        // If the parameter is not
                        if ((parameter->name() == opIter.key()) &&
                            (parameter->type() == opIter.value())) {
                            optionalParametersLeft.removeAll(parameter);
                        }
                    }

                    ++opIter;
                }

                foreach (AbstractAccountParametersWidget *widget, widgets) {
                    d->optionalParametersWidgets.append(widget);
                    d->tabWidget->addTab(widget, i18n("Optional Parameters"));
                }
            }

            // Show the generic UI if optionalParameters is not empty.
            if (optionalParametersLeft.size() > 0) {
                OptionalParameterEditWidget *opew =
                        new OptionalParameterEditWidget(optionalParametersLeft, d->tabWidget);
                d->optionalParametersWidgets.append(opew);
                d->tabWidget->addTab(opew, i18n("Optional Parameters"));
            }

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

    // Check all pages of parameters pass validation.
    if (!d->mandatoryParametersWidget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    foreach (AbstractAccountParametersWidget *w, d->optionalParametersWidgets) {
        if (!w->validateParameterValues()) {
            kDebug() << "A widget failed parameter validation. Not accepting wizard.";
            return;
        }
    }

    // Get the mandatory parameters.
    QMap<Tp::ProtocolParameter*, QVariant> mandatoryParameterValues;
    mandatoryParameterValues = d->mandatoryParametersWidget->parameterValues();

    // Get the optional properties
    QMap<Tp::ProtocolParameter*, QVariant> optionalParameterValues;

    foreach (AbstractAccountParametersWidget *w, d->optionalParametersWidgets) {
        QMap<Tp::ProtocolParameter*, QVariant> parameters = w->parameterValues();
        QMap<Tp::ProtocolParameter*, QVariant>::const_iterator i = parameters.constBegin();
        while (i != parameters.constEnd()) {
            if (!optionalParameterValues.contains(i.key())) {
                optionalParameterValues.insert(i.key(), i.value());
            } else {
                kWarning() << "Parameter:" << i.key()->name() << "is already in the map.";
            }

            ++i;
        }
        continue;
    }

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
        // TODO: User feedback in this case.
        kWarning() << "Adding Account failed:" << op->errorName() << op->errorMessage();
        return;
    }

    KAssistantDialog::accept();
}


#include "add-account-assistant.moc"

