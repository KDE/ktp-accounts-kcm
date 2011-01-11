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

#include "icq-server-settings-widget.h"

#include "ui_icq-server-settings-widget.h"

#include <KCMTelepathyAccounts/ProtocolParameterValue>

#include <KDebug>
#include <KMessageBox>


class IcqServerSettingsWidget::Private
{
public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameter serverParameter;
    Tp::ProtocolParameter portParameter;
    Tp::ProtocolParameter useSslParameter;
    Tp::ProtocolParameter alwaysUseRvProxyParameter;
    Tp::ProtocolParameter allowMultipleLoginsParameter;
    Tp::ProtocolParameter charsetParameter;

    Ui::IcqServerSettingsWidget *ui;
};

IcqServerSettingsWidget::IcqServerSettingsWidget(Tp::ProtocolParameterList parameters,
                                                     const QVariantMap &values,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(parameters, values, parent),
   d(new Private)
{
    kDebug();

    // Store the parameters this widget supports
    foreach (const Tp::ProtocolParameter &parameter, parameters) {

        kDebug() << parameter.name() << parameter.type();
        
        if ((parameter.name() == "server") && (parameter.type() == QVariant::String)) {
            d->serverParameter = parameter;
        } else if ((parameter.name() == "port") && (parameter.type() == QVariant::Int)) {
            d->portParameter = parameter;
        } else if ((parameter.name() == "use-ssl") && (parameter.type() == QVariant::Bool)) {
            d->useSslParameter = parameter;
        } else if ((parameter.name() == "always-use-rv-proxy") && (parameter.type() == QVariant::Bool)) {
            d->alwaysUseRvProxyParameter = parameter;
        } else if ((parameter.name() == "allow-multiple-logins") && (parameter.type() == QVariant::Bool)) {
            d->allowMultipleLoginsParameter = parameter;
        } else if ((parameter.name() == "charset") && (parameter.type() == QVariant::String)) {
            d->charsetParameter = parameter;
        }
    }

    // Set up the UI.
    d->ui = new Ui::IcqServerSettingsWidget;
    d->ui->setupUi(this);

    /*
     * First subformular
     */

    // Prefill UI elements if appropriate.
    if (d->serverParameter.isValid()) {
        if (values.contains(d->serverParameter.name())) {
            d->ui->serverLineEdit->setText(values.value(d->serverParameter.name()).toString());
        } else {
            d->ui->serverLineEdit->setText(d->serverParameter.defaultValue().toString());
        }
    }

    if (d->portParameter.isValid()) {
        if (values.contains(d->portParameter.name())) {
            d->ui->portSpinBox->setValue(values.value(d->portParameter.name()).toUInt());
        } else {
            d->ui->portSpinBox->setValue(d->portParameter.defaultValue().toUInt());
        }
    }

    // Hide any elements we don't have the parameters passed to show.
    if (!d->serverParameter.isValid()) {
        d->ui->serverLabel->hide();
        d->ui->serverLineEdit->hide();
    }

    if (!d->portParameter.isValid()) {
        d->ui->portLabel->hide();
        d->ui->portSpinBox->hide();
    }

    // Hide the headline if they are empty.
    if ((!d->serverParameter.isValid()) && (!d->portParameter.isValid())) {
        d->ui->serverHeadlineLabel->hide();
    }

    /*
     * Second subformular
     */
    if (d->useSslParameter.isValid()) {
        if (values.contains(d->useSslParameter.name())) {
            d->ui->useSslCheckBox->setChecked(values.value(d->useSslParameter.name()).toBool());
        } else {
            d->ui->useSslCheckBox->setChecked(d->useSslParameter.defaultValue().toBool());
        }
    }

    if (d->allowMultipleLoginsParameter.isValid()) {
        if (values.contains(d->allowMultipleLoginsParameter.name())) {
            d->ui->allowMultipleLoginsCheckBox->setChecked(values.value(d->allowMultipleLoginsParameter.name()).toBool());
        } else {
            d->ui->allowMultipleLoginsCheckBox->setChecked(d->allowMultipleLoginsParameter.defaultValue().toBool());
        }
    }

    // Hide any elements we don't have the parameters passed to show.
    if (!d->useSslParameter.isValid()) {
        d->ui->useSslCheckBox->hide();
        // It's a checkbox, so no extra label here
    }

    if (!d->allowMultipleLoginsParameter.isValid()) {
        d->ui->allowMultipleLoginsCheckBox->hide();
        // It's a checkbox, so no extra label here
    }
    
    // Hide the headline if they are empty.
    if ((!d->useSslParameter.isValid()) && (!d->allowMultipleLoginsParameter.isValid())) {
        d->ui->securityHeadlineLabel->hide();
    }

    /*
     * Third subformular
     */

    // Prefill UI elements if appropriate.
    if (d->charsetParameter.isValid()) {
        if (values.contains(d->charsetParameter.name())) {
            d->ui->charsetComboBox->setEditText(values.value(d->charsetParameter.name()).toString());
        } else {
            d->ui->charsetComboBox->setEditText(d->charsetParameter.defaultValue().toString());
        }
    }

    if (d->alwaysUseRvProxyParameter.isValid()) {
        if (values.contains(d->alwaysUseRvProxyParameter.name())) {
            d->ui->alwaysUseRvProxyCheckBox->setChecked(values.value(d->alwaysUseRvProxyParameter.name()).toBool());
        } else {
            d->ui->alwaysUseRvProxyCheckBox->setChecked(d->alwaysUseRvProxyParameter.defaultValue().toBool());
        }
    }

    // Hide any elements we don't have the parameters passed to show.
    if (!d->charsetParameter.isValid()) {
        d->ui->charsetComboBox->hide();
        // It's a checkbox, so no extra label here
    }

    if (!d->alwaysUseRvProxyParameter.isValid()) {
        d->ui->alwaysUseRvProxyCheckBox->hide();
        // It's a checkbox, so no extra label here
    }

    // Hide the headline if they are empty.
    if ((!d->charsetParameter.isValid()) && (!d->alwaysUseRvProxyParameter.isValid())) {
        d->ui->otherHeadlineLabel->hide();
    }
    
}

IcqServerSettingsWidget::~IcqServerSettingsWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> IcqServerSettingsWidget::parameterValues() const
{
    kDebug();

    QList<ProtocolParameterValue> parameters;

    // Populate the map of parameters and their values with all the parameters this widget contains.
    if (d->serverParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->serverParameter, d->ui->serverLineEdit->text()));
    }

    if (d->portParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->portParameter, d->ui->portSpinBox->value()));
    }

    if (d->useSslParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->useSslParameter, d->ui->useSslCheckBox->isChecked()));
    }

    if (d->allowMultipleLoginsParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->allowMultipleLoginsParameter, d->ui->allowMultipleLoginsCheckBox->isChecked()));
    }

    if (d->alwaysUseRvProxyParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->alwaysUseRvProxyParameter, d->ui->alwaysUseRvProxyCheckBox->isChecked()));
    }

    if (d->charsetParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->charsetParameter, d->ui->charsetComboBox->currentText()));
    }

    return parameters;
}

bool IcqServerSettingsWidget::validateParameterValues()
{
    kDebug();

    //TODO: check which parameters need validation

    return true;
}


#include "icq-server-settings-widget.moc"

