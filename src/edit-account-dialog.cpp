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

#include "edit-account-dialog.h"

#include "mandatory-parameter-edit-widget.h"
#include "optional-parameter-edit-widget.h"
#include "plugin-manager.h"

#include "libkcmtelepathyaccounts/abstract-account-parameters-widget.h"
#include "libkcmtelepathyaccounts/abstract-account-ui.h"

#include <KDebug>
#include <KLocale>
#include <KTabWidget>

#include <QtCore/QList>

#include <TelepathyQt4/ConnectionManager>
#include <TelepathyQt4/PendingStringList>

class EditAccountDialog::Private
{
public:
    Private()
            : item(0), tabWidget(0), mandatoryParametersWidget(0)
    {
        kDebug();
    }

    AccountItem *item;

    Tp::ProtocolParameterList mandatoryProtocolParameters;
    Tp::ProtocolParameterList optionalProtocolParameters;
    QVariantMap parameterValues;

    KTabWidget *tabWidget;
    AbstractAccountParametersWidget *mandatoryParametersWidget;
    QList<AbstractAccountParametersWidget*> optionalParametersWidgets;
};

EditAccountDialog::EditAccountDialog(AccountItem *item, QWidget *parent)
        : KDialog(parent),
          d(new Private)
{
    kDebug();

    // Set up the tab widget.
    d->tabWidget = new KTabWidget(this);
    setMainWidget(d->tabWidget);
    resize(400, 480);

    d->item = item;

    // Get the protocol's parameters.
    Tp::ProtocolInfo *protocolInfo = d->item->account()->protocolInfo();
    Tp::ProtocolParameterList protocolParameters = protocolInfo->parameters();

    foreach (Tp::ProtocolParameter *parameter, protocolParameters) {
        if (parameter->isRequired()) {
            d->mandatoryProtocolParameters.append(parameter);
        } else {
            d->optionalProtocolParameters.append(parameter);
        }
    }

    // Get the parameter values.
    d->parameterValues = d->item->account()->parameters();

    // The rest of this method is based on code from add-account-assistant.cpp

    // Get the AccountsUi for the plugin, and get the optional parameter widgets for it.
    AbstractAccountUi *ui = PluginManager::instance()->accountUiForProtocol(item->account()->cmName(),
                                                                            item->account()->protocol());

    // Set up the Mandatory Parameters page
    Tp::ProtocolParameterList mandatoryParametersLeft = d->mandatoryProtocolParameters;

    // Create the custom UI or generic UI depending on available parameters.
    if (ui) {
        // UI does exist, set it up.
        AbstractAccountParametersWidget *widget = ui->mandatoryParametersWidget(d->mandatoryProtocolParameters,
                                                                                item->account()->parameters());
        QMap<QString, QVariant::Type> manParams = ui->supportedMandatoryParameters();
        QMap<QString, QVariant::Type>::const_iterator manIter = manParams.constBegin();
        while(manIter != manParams.constEnd()) {
            foreach (Tp::ProtocolParameter *parameter, d->mandatoryProtocolParameters) {
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
        d->mandatoryParametersWidget = new MandatoryParameterEditWidget(
                d->mandatoryProtocolParameters,
                item->account()->parameters(),
                d->tabWidget);
    }

    d->tabWidget->addTab(d->mandatoryParametersWidget, i18n("Mandatory Parameters"));

    // Get the list of parameters
    Tp::ProtocolParameterList optionalParametersLeft = d->optionalProtocolParameters;

    // Check if the AbstractAccountUi exists. If not then we use the autogenerated UI for
    // everything.
    if (ui) {
        // UI Does exist, set it up.
        QList<AbstractAccountParametersWidget*> widgets = ui->optionalParametersWidgets(
                d->optionalProtocolParameters,
                item->account()->parameters());

        // Remove all handled parameters from the optionalParameters list.
        QMap<QString, QVariant::Type> opParams = ui->supportedOptionalParameters();
        QMap<QString, QVariant::Type>::const_iterator opIter = opParams.constBegin();
        while(opIter != opParams.constEnd()) {
            foreach (Tp::ProtocolParameter *parameter, d->optionalProtocolParameters) {
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
                new OptionalParameterEditWidget(optionalParametersLeft,
                                                item->account()->parameters(),
                                                d->tabWidget);
        d->optionalParametersWidgets.append(opew);
        d->tabWidget->addTab(opew, i18n("Optional Parameters"));
    }

}

EditAccountDialog::~EditAccountDialog()
{
    kDebug();
}

void EditAccountDialog::slotButtonClicked(int button)
{
    kDebug() << "Button clicked:" << button;

    if (button == KDialog::Ok) {
        accept();
    } else{
        KDialog::slotButtonClicked(button);
    }
}

void EditAccountDialog::accept()
{
    kDebug();

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

    // Get the optional parameters.
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

    // Merge the parameters into a QVariantMap for submitting to the Telepathy AM.
    QVariantMap parameters;
    QStringList unsetParameters;

    foreach (Tp::ProtocolParameter *pp, mandatoryParameterValues.keys()) {
        QVariant value = mandatoryParameterValues.value(pp);

        // Unset empty parameters.
        if (!value.isNull()) {
            parameters.insert(pp->name(), value);
            continue;
        }

        unsetParameters.append(pp->name());
    }

    foreach (Tp::ProtocolParameter *pp, optionalParameterValues.keys()) {
        QVariant value = optionalParameterValues.value(pp);

        // Unset empty parameters.
        if (!value.isNull()) {
            parameters.insert(pp->name(), value);
            continue;
        }

        unsetParameters.append(pp->name());
    }

    // FIXME: Ask the user to submit a Display Name
    Tp::PendingStringList *psl = d->item->account()->updateParameters(parameters, unsetParameters);

    connect(psl,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onParametersUpdated(Tp::PendingOperation*)));
}

void EditAccountDialog::onParametersUpdated(Tp::PendingOperation *op)
{
    kDebug();

    if (op->isError()) {
        // FIXME: Visual feedback in GUI to user.
        kWarning() << "Could not update parameters:" << op->errorName() << op->errorMessage();
        return;
    }

    emit finished();
}


#include "edit-account-dialog.moc"

