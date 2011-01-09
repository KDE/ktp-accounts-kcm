/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "account-edit-widget.h"
#include "ui_account-edit-widget.h"

#include "abstract-account-parameters-widget.h"
#include "abstract-account-ui.h"
#include "parameter-edit-widget.h"
#include "plugin-manager.h"
#include "dictionary.h"

#include <KDebug>
#include <KLocale>

#include <QtCore/QList>

class AccountEditWidget::Private
{
public:
    Private()
            : accountUi(0), mainOptionsWidget(0)
    {
        kDebug();
    }

    QString connectionManager;
    QString protocol;
    Tp::ProtocolParameterList parameters;
    QVariantMap parameterValues;

    AbstractAccountUi *accountUi;
    Ui::AccountEditWidget *ui;
    AbstractAccountParametersWidget *mainOptionsWidget;
    QList<ProtocolParameterValue> advancedParameterValues;
};

AccountEditWidget::AccountEditWidget(const Tp::ProtocolInfo &info,
                                     const QVariantMap &parameterValues,
                                     QWidget *parent)
        : QWidget(parent),
          d(new Private)
{
    kDebug();

    // Set up the interface
    d->ui = new Ui::AccountEditWidget;
    d->ui->setupUi(this);

    d->connectionManager = info.cmName();
    d->protocol = info.name();
    d->parameters = info.parameters();
    d->parameterValues = parameterValues;

    connect(d->ui->advancedButton, SIGNAL(clicked()),
            this, SLOT(onAdvancedClicked()));

    d->ui.advancedButton->setIcon(KIcon("configure"));
    d->ui.titleLabel->setText(Dictionary::instance()->string(info.name()));
    d->ui.iconLabel->setText("");
    d->ui.iconLabel->setPixmap(KIcon(info.iconName()).pixmap(32));

    loadWidgets();
}

AccountEditWidget::~AccountEditWidget()
{
    kDebug();

    delete d->ui;
    delete d;
}

bool AccountEditWidget::validateParameterValues() const
{
    // the parameters handle by the advanced dialog are validated when the
    // dialog exits, so no need to validate them here
    return d->mainOptionsWidget->validateParameterValues();
}

QList<ProtocolParameterValue> AccountEditWidget::parameterValues() const
{
    QList<ProtocolParameterValue> values;
    values = d->mainOptionsWidget->parameterValues();

    // append the advanced options, if any
    values.append(d->advancedParameterValues);
    return values;
}

void AccountEditWidget::loadWidgets()
{
    Tp::ProtocolParameterList mandatoryParameters;

    foreach (const Tp::ProtocolParameter &parameter, d->parameters) {
        if (parameter.isRequired()) {
            mandatoryParameters.append(parameter);
        }
    }

    // Get the AccountsUi for the plugin, and get the optional parameter widgets for it.
    d->accountUi = PluginManager::instance()->accountUiForProtocol(d->connectionManager,
                                                                   d->protocol);

    // Create the custom UI or generic UI depending on available parameters.
    if (d->accountUi) {
        // UI does exist, set it up.
        d->mainOptionsWidget = d->accountUi->mainOptionsWidget(d->parameters,
                                                               d->parameterValues,
                                                               this);
        //Widgets wrapped in a layout should not have double margins
        d->mainOptionsWidget->layout()->setContentsMargins(0, 0, 0, 0);
        d->ui->advancedButton->setVisible(d->accountUi->hasAdvancedOptionsWidget());
        d->ui->verticalLayout->insertWidget(0, d->mainOptionsWidget);

        // check if all the parameters the UI supports are available in the CM plugin
        // also verify if the UI handle all mandatory parameters
        QMap<QString, QVariant::Type> params = d->accountUi->supportedParameters();
        QMap<QString, QVariant::Type>::const_iterator paramIter = params.constBegin();
        bool paramFound = false;
        while(paramIter != params.constEnd()) {
            paramFound = false;
            foreach (const Tp::ProtocolParameter &parameter, d->parameters) {
                if ((parameter.name() == paramIter.key()) &&
                    (parameter.type() == paramIter.value())) {
                    mandatoryParameters.removeAll(parameter);
                    paramFound = true;
                }
            }
            if (!paramFound)
                 break;

            ++paramIter;
        }

        // if a parameter is missing or a mandatory parameter is not being handle,
        // fall back to the generic interface.
        // FIXME: not sure this is the best way to handle the situation, but
        //        it is like this in a hope the case won't happen often
        if (!paramFound || !mandatoryParameters.isEmpty()) {
            delete d->mainOptionsWidget;
            d->mainOptionsWidget = 0;
        }
    }

    if (!d->mainOptionsWidget) {
        d->mainOptionsWidget = new ParameterEditWidget(
                d->parameters,
                d->parameterValues,
                this);
        d->ui->advancedButton->setVisible(false);
        d->ui->verticalLayout->insertWidget(0, d->mainOptionsWidget);
        d->ui->verticalLayout->setStretch(0, 1);
    }
}

void AccountEditWidget::onAdvancedClicked()
{
    KDialog dialog(this);
    dialog.setWindowTitle(i18n("Advanced Options"));

    AbstractAccountParametersWidget *advancedWidget;
    advancedWidget = d->accountUi->advancedOptionsWidget(d->parameters,
                                                         d->parameterValues,
                                                         &dialog);
    dialog.setMainWidget(advancedWidget);

    // loop until the entered values are ok or the user cancels the dialog

    while(true) {
        if (dialog.exec() == KDialog::Accepted) {
            // validate the parameter values
            if (!advancedWidget->validateParameterValues())
                continue;

            // at this point the values are fine
            d->advancedParameterValues = advancedWidget->parameterValues();
            // update the parameter values in case the dialog is opened again

            foreach(const ProtocolParameterValue &ppv, d->advancedParameterValues)
            {
                d->parameterValues.insert(ppv.name(), ppv.value());
            }
            break;
        }
        else
            break;
    }
}

#include "account-edit-widget.moc"

