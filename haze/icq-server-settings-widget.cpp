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
    Tp::ProtocolParameter keepaliveIntervalParameter;
    Tp::ProtocolParameter lowBandwidthParameter;
    Tp::ProtocolParameter requireEncryptionParameter;
    Tp::ProtocolParameter ignoreSslErrorsParameter;
    Tp::ProtocolParameter oldSslParameter;

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
        if ((parameter.name() == "server") && (parameter.type() == QVariant::String)) {
            d->serverParameter = parameter;
        } else if ((parameter.name() == "port") && (parameter.type() == QVariant::UInt)) {
            d->portParameter = parameter;
        } else if ((parameter.name() == "keepalive-interval") && (parameter.type() == QVariant::UInt)) {
            d->keepaliveIntervalParameter = parameter;
        } else if ((parameter.name() == "low-bandwidth") && (parameter.type() == QVariant::Bool)) {
            d->lowBandwidthParameter = parameter;
        } else if ((parameter.name() == "require-encryption") && (parameter.type() == QVariant::Bool)) {
            d->requireEncryptionParameter = parameter;
        } else if ((parameter.name() == "ignore-ssl-errors") && (parameter.type() == QVariant::Bool)) {
            d->ignoreSslErrorsParameter = parameter;
        } else if ((parameter.name() == "old-ssl") && (parameter.type() == QVariant::Bool)) {
            d->oldSslParameter = parameter;
        }
    }

    // Set up the UI.
    d->ui = new Ui::IcqServerSettingsWidget;
    d->ui->setupUi(this);

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

    if (d->keepaliveIntervalParameter.isValid()) {
        if (values.contains(d->keepaliveIntervalParameter.name())) {
            d->ui->keepaliveIntervalSpinBox->setValue(values.value(
                    d->keepaliveIntervalParameter.name()).toUInt());
        } else {
            d->ui->keepaliveIntervalSpinBox->setValue(
                    d->keepaliveIntervalParameter.defaultValue().toUInt());
        }
    }

    if (d->lowBandwidthParameter.isValid()) {
        if (values.contains(d->lowBandwidthParameter.name())) {
            d->ui->lowBandwidthCheckBox->setChecked(values.value(
                    d->lowBandwidthParameter.name()).toBool());
        } else {
            d->ui->lowBandwidthCheckBox->setChecked(
                    d->lowBandwidthParameter.defaultValue().toBool());
        }
    }

    if (d->requireEncryptionParameter.isValid()) {
        if (values.contains(d->requireEncryptionParameter.name())) {
            d->ui->requireEncryptionCheckBox->setChecked(values.value(
                    d->requireEncryptionParameter.name()).toBool());
        } else {
            d->ui->requireEncryptionCheckBox->setChecked(
                    d->requireEncryptionParameter.defaultValue().toBool());
        }
    }

    if (d->ignoreSslErrorsParameter.isValid()) {
        if (values.contains(d->ignoreSslErrorsParameter.name())) {
            d->ui->ignoreSslErrorsCheckBox->setChecked(values.value(
                    d->ignoreSslErrorsParameter.name()).toBool());
        } else {
            d->ui->ignoreSslErrorsCheckBox->setChecked(
                    d->ignoreSslErrorsParameter.defaultValue().toBool());
        }
    }

    if (d->oldSslParameter.isValid()) {
        if (values.contains(d->oldSslParameter.name())) {
            d->ui->oldSslCheckBox->setChecked(values.value(d->oldSslParameter.name()).toBool());
        } else {
            d->ui->oldSslCheckBox->setChecked(d->oldSslParameter.defaultValue().toBool());
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

    if (!d->keepaliveIntervalParameter.isValid()) {
        d->ui->keepaliveIntervalLabel->hide();
        d->ui->keepaliveIntervalSpinBox->hide();
    }

    if (!d->lowBandwidthParameter.isValid()) {
        d->ui->lowBandwidthCheckBox->hide();
    }

    if (!d->requireEncryptionParameter.isValid()) {
        d->ui->requireEncryptionCheckBox->hide();
    }

    if (!d->ignoreSslErrorsParameter.isValid()) {
        d->ui->ignoreSslErrorsCheckBox->hide();
    }

    if (!d->oldSslParameter.isValid()) {
        d->ui->oldSslCheckBox->hide();
    }

    // Hide the group boxes if they are empty.
    if ((!d->serverParameter.isValid()) && (!d->portParameter.isValid())) {
        d->ui->serverGroupBox->hide();
    }

    if ((!d->keepaliveIntervalParameter.isValid()) && (!d->lowBandwidthParameter.isValid())) {
        d->ui->connectionGroupBox->hide();
    }

    if ((!d->requireEncryptionParameter.isValid()) &&
        (!d->ignoreSslErrorsParameter.isValid()) &&
        (!d->oldSslParameter.isValid())) {
        d->ui->securityGroupBox->hide();
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

    if (d->keepaliveIntervalParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->keepaliveIntervalParameter, d->ui->keepaliveIntervalSpinBox->value()));
    }

    if (d->lowBandwidthParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->lowBandwidthParameter, d->ui->lowBandwidthCheckBox->isChecked()));
    }

    if (d->requireEncryptionParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->requireEncryptionParameter,
                          d->ui->requireEncryptionCheckBox->isChecked()));
    }

    if (d->ignoreSslErrorsParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->ignoreSslErrorsParameter, d->ui->ignoreSslErrorsCheckBox->isChecked()));
    }

    if (d->oldSslParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->oldSslParameter, d->ui->oldSslCheckBox->isChecked()));
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

