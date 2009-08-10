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

#include "server-settings-widget.h"

#include "ui_server-settings-widget.h"

#include <KDebug>
#include <KMessageBox>

class ServerSettingsWidget::Private
{
public:
    Private()
            : serverParameter(0),
              portParameter(0),
              keepaliveIntervalParameter(0),
              lowBandwidthParameter(0),
              requireEncryptionParameter(0),
              ignoreSslErrorsParameter(0),
              oldSslParameter(0),
              ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameterList parameters;
    Tp::ProtocolParameter *serverParameter;
    Tp::ProtocolParameter *portParameter;
    Tp::ProtocolParameter *keepaliveIntervalParameter;
    Tp::ProtocolParameter *lowBandwidthParameter;
    Tp::ProtocolParameter *requireEncryptionParameter;
    Tp::ProtocolParameter *ignoreSslErrorsParameter;
    Tp::ProtocolParameter *oldSslParameter;

    Ui::ServerSettingsWidget *ui;
};

ServerSettingsWidget::ServerSettingsWidget(Tp::ProtocolParameterList parameters,
                                                     const QVariantMap &values,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(parameters, values, parent),
   d(new Private)
{
    kDebug();

    // Save the parameters.
    d->parameters = parameters;

    // Store the parameters this widget supports
    foreach (Tp::ProtocolParameter *parameter, d->parameters) {
        if ((parameter->name() == "server") && (parameter->type() == QVariant::String)) {
            if (!d->serverParameter) {
                d->serverParameter = parameter;
            }
        } else if ((parameter->name() == "port") && (parameter->type() == QVariant::UInt)) {
            if (!d->portParameter) {
                d->portParameter = parameter;
            }
        } else if ((parameter->name() == "keepalive-interval") && (parameter->type() == QVariant::UInt)) {
            if (!d->keepaliveIntervalParameter) {
                d->keepaliveIntervalParameter = parameter;
            }
        } else if ((parameter->name() == "low-bandwidth") && (parameter->type() == QVariant::Bool)) {
            if (!d->lowBandwidthParameter) {
                d->lowBandwidthParameter = parameter;
            }
        } else if ((parameter->name() == "require-encryption") && (parameter->type() == QVariant::Bool)) {
            if (!d->requireEncryptionParameter) {
                d->requireEncryptionParameter = parameter;
            }
        } else if ((parameter->name() == "ignore-ssl-errors") && (parameter->type() == QVariant::Bool)) {
            if (!d->ignoreSslErrorsParameter) {
                d->ignoreSslErrorsParameter = parameter;
            }
        } else if ((parameter->name() == "old-ssl") && (parameter->type() == QVariant::Bool)) {
            if (!d->oldSslParameter) {
                d->oldSslParameter = parameter;
            }
        }
    }

    // Set up the UI.
    d->ui = new Ui::ServerSettingsWidget;
    d->ui->setupUi(this);

    // Prefill UI elements if appropriate.
    if (d->serverParameter) {
        if (values.contains(d->serverParameter->name())) {
            d->ui->serverLineEdit->setText(values.value(d->serverParameter->name()).toString());
        }
    }

    if (d->portParameter) {
        if (values.contains(d->portParameter->name())) {
            d->ui->portLineEdit->setValue(values.value(d->portParameter->name()).toUInt());
        }
    }

    if (d->keepaliveIntervalParameter) {
        if (values.contains(d->keepaliveIntervalParameter->name())) {
            d->ui->keepaliveIntervalLineEdit->setValue(values.value(
                    d->keepaliveIntervalParameter->name()).toUInt());
        }
    }

    if (d->lowBandwidthParameter) {
        if (values.contains(d->lowBandwidthParameter->name())) {
            d->ui->lowBandwidthCheckBox->setChecked(values.value(
                    d->lowBandwidthParameter->name()).toBool());
        }
    }

    if (d->requireEncryptionParameter) {
        if (values.contains(d->requireEncryptionParameter->name())) {
            d->ui->requireEncryptionCheckBox->setChecked(values.value(
                    d->requireEncryptionParameter->name()).toBool());
        }
    }

    if (d->ignoreSslErrorsParameter) {
        if (values.contains(d->ignoreSslErrorsParameter->name())) {
            d->ui->ignoreSslErrorsCheckBox->setChecked(values.value(
                    d->ignoreSslErrorsParameter->name()).toBool());
        }
    }

    if (d->oldSslParameter) {
        if (values.contains(d->oldSslParameter->name())) {
            d->ui->oldSslCheckBox->setChecked(values.value(d->oldSslParameter->name()).toBool());
        }
    }

    // Hide any elements we don't have the parameters passed to show.
    if (!d->serverParameter) {
        d->ui->serverLabel->hide();
        d->ui->serverLineEdit->hide();
    }

    if (!d->portParameter) {
        d->ui->portLabel->hide();
        d->ui->portLineEdit->hide();
    }

    if (!d->keepaliveIntervalParameter) {
        d->ui->keepaliveIntervalLabel->hide();
        d->ui->keepaliveIntervalLineEdit->hide();
    }

    if (!d->lowBandwidthParameter) {
        d->ui->lowBandwidthCheckBox->hide();
    }

    if (!d->requireEncryptionParameter) {
        d->ui->requireEncryptionCheckBox->hide();
    }

    if (!d->ignoreSslErrorsParameter) {
        d->ui->ignoreSslErrorsCheckBox->hide();
    }

    if (!d->oldSslParameter) {
        d->ui->oldSslCheckBox->hide();
    }

    // Hide the group boxes if they are empty.
    if ((!d->serverParameter) && (!d->portParameter)) {
        d->ui->serverGroupBox->hide();
    }

    if ((!d->keepaliveIntervalParameter) && (!d->lowBandwidthParameter)) {
        d->ui->connectionGroupBox->hide();
    }

    if ((!d->requireEncryptionParameter) &&
        (!d->ignoreSslErrorsParameter) &&
        (!d->oldSslParameter)) {
        d->ui->securityGroupBox->hide();
    }
}

ServerSettingsWidget::~ServerSettingsWidget()
{
    kDebug();

    delete d;
}

QMap<Tp::ProtocolParameter*, QVariant> ServerSettingsWidget::parameterValues() const
{
    kDebug();

    QMap<Tp::ProtocolParameter*, QVariant> parameters;

    // Populate the map of parameters and their values with all the parameters this widget contains.
    if (d->serverParameter) {
        parameters.insert(d->serverParameter, d->ui->serverLineEdit->text());
    }

    if (d->portParameter) {
        parameters.insert(d->portParameter, d->ui->portLineEdit->value());
    }

    if (d->keepaliveIntervalParameter) {
        parameters.insert(d->keepaliveIntervalParameter, d->ui->keepaliveIntervalLineEdit->value());
    }

    if (d->lowBandwidthParameter) {
        parameters.insert(d->lowBandwidthParameter, d->ui->lowBandwidthCheckBox->isChecked());
    }

    if (d->requireEncryptionParameter) {
        parameters.insert(d->requireEncryptionParameter,
                          d->ui->requireEncryptionCheckBox->isChecked());
    }

    if (d->ignoreSslErrorsParameter) {
        parameters.insert(d->ignoreSslErrorsParameter, d->ui->ignoreSslErrorsCheckBox->isChecked());
    }

    if (d->oldSslParameter) {
        parameters.insert(d->oldSslParameter, d->ui->oldSslCheckBox->isChecked());
    }

    return parameters;
}

bool ServerSettingsWidget::validateParameterValues()
{
    kDebug();

    return true;
}


#include "server-settings-widget.moc"

