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

#include "proxy-settings-widget.h"

#include "ui_proxy-settings-widget.h"

#include <KCMTelepathyAccounts/ProtocolParameterValue>

#include <KDebug>
#include <KMessageBox>


class ProxySettingsWidget::Private
{
public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameter stunServerParameter;
    Tp::ProtocolParameter stunPortParameter;
    Tp::ProtocolParameter fallbackStunServerParameter;
    Tp::ProtocolParameter fallbackStunPortParameter;
    Tp::ProtocolParameter httpsProxyServerParameter;
    Tp::ProtocolParameter httpsProxyPortParameter;
    Tp::ProtocolParameter fallbackSocks5ProxiesParameter;
    Tp::ProtocolParameter fallbackConferenceServerParameter;

    Ui::ProxySettingsWidget *ui;
};

ProxySettingsWidget::ProxySettingsWidget(Tp::ProtocolParameterList parameters,
                                         const QVariantMap &values,
                                         QWidget *parent)
 : AbstractAccountParametersWidget(parameters, values, parent),
   d(new Private)
{
    kDebug();

    // Store the parameters this widget supports
    foreach (const Tp::ProtocolParameter &parameter, parameters) {
        if ((parameter.name() == "stun-server") && (parameter.type() == QVariant::String)) {
                d->stunServerParameter = parameter;
        } else if ((parameter.name() == "stun-port") && (parameter.type() == QVariant::UInt)) {
                d->stunPortParameter = parameter;
        } else if ((parameter.name() == "fallback-stun-server") &&
                   (parameter.type() == QVariant::String)) {
                d->fallbackStunServerParameter = parameter;
        } else if ((parameter.name() == "fallback-stun-port") &&
                   (parameter.type() == QVariant::UInt)) {
                d->fallbackStunPortParameter = parameter;
        } else if ((parameter.name() == "https-proxy-server") &&
                   (parameter.type() == QVariant::String)) {
                d->httpsProxyServerParameter = parameter;
        } else if ((parameter.name() == "https-proxy-port") &&
                   (parameter.type() == QVariant::UInt)) {
                d->httpsProxyPortParameter = parameter;
        } else if ((parameter.name() == "fallback-socks5-proxies") &&
                   (parameter.type() == QVariant::StringList)) {
                d->fallbackSocks5ProxiesParameter = parameter;
        } else if ((parameter.name() == "fallback-conference-server") &&
                   (parameter.type() == QVariant::String)) {
                d->fallbackConferenceServerParameter = parameter;
        }
    }

    // Set up the UI.
    d->ui = new Ui::ProxySettingsWidget;
    d->ui->setupUi(this);

    // Prefill UI elements if appropriate.
    if (d->stunServerParameter.isValid()) {
        if (values.contains(d->stunServerParameter.name())) {
            d->ui->stunServerLineEdit->setText(values.value(
                    d->stunServerParameter.name()).toString());
        } else {
            d->ui->stunServerLineEdit->setText(d->stunServerParameter.defaultValue().toString());
        }
    }

    if (d->stunPortParameter.isValid()) {
        if (values.contains(d->stunPortParameter.name())) {
            d->ui->stunPortSpinBox->setValue(values.value(d->stunPortParameter.name()).toUInt());
        } else {
            d->ui->stunPortSpinBox->setValue(d->stunPortParameter.defaultValue().toUInt());
        }
    }

    if (d->fallbackStunServerParameter.isValid()) {
        if (values.contains(d->fallbackStunServerParameter.name())) {
            d->ui->fallbackStunServerLineEdit->setText(values.value(
                    d->fallbackStunServerParameter.name()).toString());
        } else {
            d->ui->fallbackStunServerLineEdit->setText(
                    d->fallbackStunServerParameter.defaultValue().toString());
        }
    }

    if (d->fallbackStunPortParameter.isValid()) {
        if (values.contains(d->fallbackStunPortParameter.name())) {
            d->ui->fallbackStunPortSpinBox->setValue(values.value(
                    d->fallbackStunPortParameter.name()).toUInt());
        } else {
            d->ui->fallbackStunPortSpinBox->setValue(
                    d->fallbackStunPortParameter.defaultValue().toUInt());
        }
    }

    if (d->httpsProxyServerParameter.isValid()) {
        if (values.contains(d->httpsProxyServerParameter.name())) {
            d->ui->httpsProxyServerLineEdit->setText(values.value(
                    d->httpsProxyServerParameter.name()).toString());
        } else {
            d->ui->httpsProxyServerLineEdit->setText(
                    d->httpsProxyServerParameter.defaultValue().toString());
        }
    }

    if (d->httpsProxyPortParameter.isValid()) {
        if (values.contains(d->httpsProxyPortParameter.name())) {
            d->ui->httpsProxyPortSpinBox->setValue(values.value(
                    d->httpsProxyPortParameter.name()).toUInt());
        } else {
            d->ui->httpsProxyPortSpinBox->setValue(
                    d->httpsProxyPortParameter.defaultValue().toUInt());
        }
    }

    if (d->fallbackSocks5ProxiesParameter.isValid()) {
        if (values.contains(d->fallbackSocks5ProxiesParameter.name())) {
            foreach (const QString &line,
                     values.value(d->fallbackSocks5ProxiesParameter.name()).toStringList()) {
                d->ui->fallbackSocks5ProxiesTextEdit->append(line);
            }
        } else {
            foreach (const QString &line,
                     d->fallbackSocks5ProxiesParameter.defaultValue().toStringList()) {
                d->ui->fallbackSocks5ProxiesTextEdit->append(line);
            }
        }
    }

    if (d->fallbackConferenceServerParameter.isValid()) {
        if (values.contains(d->fallbackConferenceServerParameter.name())) {
            d->ui->fallbackConferenceServerLineEdit->setText(values.value(
                    d->fallbackConferenceServerParameter.name()).toString());
        } else {
            d->ui->fallbackConferenceServerLineEdit->setText(
                    d->fallbackConferenceServerParameter.defaultValue().toString());
        }
    }

    // Hide any elements we don't have the parameters passed to show.
    if (!d->stunServerParameter.isValid()) {
        d->ui->stunServerLabel->hide();
        d->ui->stunServerLineEdit->hide();
    }

    if (!d->stunPortParameter.isValid()) {
        d->ui->stunPortLabel->hide();
        d->ui->stunPortSpinBox->hide();
    }

    if (!d->fallbackStunServerParameter.isValid()) {
        d->ui->fallbackStunServerLabel->hide();
        d->ui->fallbackStunServerLineEdit->hide();
    }

    if (!d->fallbackStunPortParameter.isValid()) {
        d->ui->fallbackStunPortLabel->hide();
        d->ui->fallbackStunPortSpinBox->hide();
    }

    if (!d->httpsProxyServerParameter.isValid()) {
        d->ui->httpsProxyServerLabel->hide();
        d->ui->httpsProxyServerLineEdit->hide();
    }

    if (!d->httpsProxyPortParameter.isValid()) {
        d->ui->httpsProxyPortLabel->hide();
        d->ui->httpsProxyPortSpinBox->hide();
    }

    if (!d->fallbackSocks5ProxiesParameter.isValid()) {
        d->ui->fallbackSocks5ProxiesLabel->hide();
        d->ui->fallbackSocks5ProxiesTextEdit->hide();
    }

    if (!d->fallbackConferenceServerParameter.isValid()) {
        d->ui->fallbackConferenceServerLabel->hide();
        d->ui->fallbackConferenceServerLineEdit->hide();
    }

    // Hide the group boxes if they are empty.
    if ((!d->stunServerParameter.isValid()) &&
        (!d->stunPortParameter.isValid()) &&
        (!d->fallbackStunServerParameter.isValid()) &&
        (!d->fallbackStunPortParameter.isValid())) {
        d->ui->stunGroupBox->hide();
    }

    if ((!d->httpsProxyServerParameter.isValid()) &&
        (!d->httpsProxyPortParameter.isValid()) &&
        (!d->fallbackSocks5ProxiesParameter.isValid())) {
        d->ui->proxyGroupBox->hide();
    }

    if (!d->fallbackConferenceServerParameter.isValid()) {
        d->ui->conferenceGroupBox->hide();
    }
}

ProxySettingsWidget::~ProxySettingsWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> ProxySettingsWidget::parameterValues() const
{
    kDebug();

    QList<ProtocolParameterValue> parameters;

    // Populate the map of parameters and their values with all the parameters this widget contains.
    if (d->stunServerParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->stunServerParameter, d->ui->stunServerLineEdit->text()));
    }

    if (d->stunPortParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->stunPortParameter, d->ui->stunPortSpinBox->value()));
    }

    if (d->fallbackStunServerParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->fallbackStunServerParameter, d->ui->fallbackStunServerLineEdit->text()));
    }

    if (d->fallbackStunPortParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->fallbackStunPortParameter, d->ui->fallbackStunPortSpinBox->value()));
    }

    if (d->httpsProxyServerParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->httpsProxyServerParameter, d->ui->httpsProxyServerLineEdit->text()));
    }

    if (d->httpsProxyPortParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->httpsProxyPortParameter, d->ui->httpsProxyPortSpinBox->value()));
    }

    if (d->fallbackSocks5ProxiesParameter.isValid()) {
        QString text = d->ui->fallbackSocks5ProxiesTextEdit->toPlainText();
        QStringList value = text.split("\n", QString::SkipEmptyParts);
        parameters.append(ProtocolParameterValue(d->fallbackSocks5ProxiesParameter, value));
    }

    if (d->fallbackConferenceServerParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->fallbackConferenceServerParameter,
                                                 d->ui->fallbackConferenceServerLineEdit->text()));
    }

    return parameters;
}

bool ProxySettingsWidget::validateParameterValues()
{
    kDebug();

    //TODO: check what parameters should be validated

    return true;
}


#include "proxy-settings-widget.moc"

