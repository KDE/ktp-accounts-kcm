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

#include <KDebug>
#include <KMessageBox>

class ProxySettingsWidget::Private
{
public:
    Private()
            : stunServerParameter(0),
              stunPortParameter(0),
              fallbackStunServerParameter(0),
              fallbackStunPortParameter(0),
              httpsProxyServerParameter(0),
              httpsProxyPortParameter(0),
              fallbackSocks5ProxiesParameter(0),
              fallbackConferenceServerParameter(0),
              ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameterList parameters;
    Tp::ProtocolParameter *stunServerParameter;
    Tp::ProtocolParameter *stunPortParameter;
    Tp::ProtocolParameter *fallbackStunServerParameter;
    Tp::ProtocolParameter *fallbackStunPortParameter;
    Tp::ProtocolParameter *httpsProxyServerParameter;
    Tp::ProtocolParameter *httpsProxyPortParameter;
    Tp::ProtocolParameter *fallbackSocks5ProxiesParameter;
    Tp::ProtocolParameter *fallbackConferenceServerParameter;

    Ui::ProxySettingsWidget *ui;
};

ProxySettingsWidget::ProxySettingsWidget(Tp::ProtocolParameterList parameters,
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
        if ((parameter->name() == "stun-server") && (parameter->type() == QVariant::String)) {
            if (!d->stunServerParameter) {
                d->stunServerParameter = parameter;
            }
        } else if ((parameter->name() == "stun-port") && (parameter->type() == QVariant::UInt)) {
            if (!d->stunPortParameter) {
                d->stunPortParameter = parameter;
            }
        } else if ((parameter->name() == "fallback-stun-server") &&
                   (parameter->type() == QVariant::String)) {
            if (!d->fallbackStunServerParameter) {
                d->fallbackStunServerParameter = parameter;
            }
        } else if ((parameter->name() == "fallback-stun-port") &&
                   (parameter->type() == QVariant::UInt)) {
            if (!d->fallbackStunPortParameter) {
                d->fallbackStunPortParameter = parameter;
            }
        } else if ((parameter->name() == "https-proxy-server") &&
                   (parameter->type() == QVariant::String)) {
            if (!d->httpsProxyServerParameter) {
                d->httpsProxyServerParameter = parameter;
            }
        } else if ((parameter->name() == "https-proxy-port") &&
                   (parameter->type() == QVariant::UInt)) {
            if (!d->httpsProxyPortParameter) {
                d->httpsProxyPortParameter = parameter;
            }
        } else if ((parameter->name() == "fallback-socks5-proxies") &&
                   (parameter->type() == QVariant::StringList)) {
            if (!d->fallbackSocks5ProxiesParameter) {
                d->fallbackSocks5ProxiesParameter = parameter;
            }
        } else if ((parameter->name() == "fallback-conference-server") &&
                   (parameter->type() == QVariant::String)) {
            if (!d->fallbackConferenceServerParameter) {
                d->fallbackConferenceServerParameter = parameter;
            }
        }
    }

    // Set up the UI.
    d->ui = new Ui::ProxySettingsWidget;
    d->ui->setupUi(this);

    // Prefill UI elements if appropriate.
    if (d->stunServerParameter) {
        if (values.contains(d->stunServerParameter->name())) {
            d->ui->stunServerLineEdit->setText(values.value(
                    d->stunServerParameter->name()).toString());
        }
    }

    if (d->stunPortParameter) {
        if (values.contains(d->stunPortParameter->name())) {
            d->ui->stunPortLineEdit->setValue(values.value(d->stunPortParameter->name()).toUInt());
        }
    }

    if (d->fallbackStunServerParameter) {
        if (values.contains(d->fallbackStunServerParameter->name())) {
            d->ui->fallbackStunServerLineEdit->setText(values.value(
                    d->fallbackStunServerParameter->name()).toString());
        }
    }

    if (d->fallbackStunPortParameter) {
        if (values.contains(d->fallbackStunPortParameter->name())) {
            d->ui->fallbackStunPortLineEdit->setValue(values.value(
                    d->fallbackStunPortParameter->name()).toUInt());
        }
    }

    if (d->httpsProxyServerParameter) {
        if (values.contains(d->httpsProxyServerParameter->name())) {
            d->ui->httpsProxyServerLineEdit->setText(values.value(
                    d->httpsProxyServerParameter->name()).toString());
        }
    }

    if (d->httpsProxyPortParameter) {
        if (values.contains(d->httpsProxyPortParameter->name())) {
            d->ui->httpsProxyPortLineEdit->setValue(values.value(
                    d->httpsProxyPortParameter->name()).toUInt());
        }
    }

    if (d->fallbackSocks5ProxiesParameter) {
        if (values.contains(d->fallbackSocks5ProxiesParameter->name())) {
            foreach (const QString &line,
                     values.value(d->fallbackSocks5ProxiesParameter->name()).toStringList()) {
                d->ui->fallbackSocks5ProxiesTextEdit->append(line);
            }
        }
    }

    if (d->fallbackConferenceServerParameter) {
        if (values.contains(d->fallbackConferenceServerParameter->name())) {
            d->ui->fallbackConferenceServerLineEdit->setText(values.value(
                    d->fallbackConferenceServerParameter->name()).toString());
        }
    }

    // Hide any elements we don't have the parameters passed to show.
    if (!d->stunServerParameter) {
        d->ui->stunServerLabel->hide();
        d->ui->stunServerLineEdit->hide();
    }

    if (!d->stunPortParameter) {
        d->ui->stunPortLabel->hide();
        d->ui->stunPortLineEdit->hide();
    }

    if (!d->fallbackStunServerParameter) {
        d->ui->fallbackStunServerLabel->hide();
        d->ui->fallbackStunServerLineEdit->hide();
    }

    if (!d->fallbackStunPortParameter) {
        d->ui->fallbackStunPortLabel->hide();
        d->ui->fallbackStunPortLineEdit->hide();
    }

    if (!d->httpsProxyServerParameter) {
        d->ui->httpsProxyServerLabel->hide();
        d->ui->httpsProxyServerLineEdit->hide();
    }

    if (!d->httpsProxyPortParameter) {
        d->ui->httpsProxyPortLabel->hide();
        d->ui->httpsProxyPortLineEdit->hide();
    }

    if (!d->fallbackSocks5ProxiesParameter) {
        d->ui->fallbackSocks5ProxiesLabel->hide();
        d->ui->fallbackSocks5ProxiesTextEdit->hide();
    }

    if (!d->fallbackConferenceServerParameter) {
        d->ui->fallbackConferenceServerLabel->hide();
        d->ui->fallbackConferenceServerLineEdit->hide();
    }

    // Hide the group boxes if they are empty.
    if ((!d->stunServerParameter) &&
        (!d->stunPortParameter) &&
        (!d->fallbackStunServerParameter) &&
        (!d->fallbackStunPortParameter)) {
        d->ui->stunGroupBox->hide();
    }

    if ((!d->httpsProxyServerParameter) &&
        (!d->httpsProxyPortParameter) &&
        (!d->fallbackSocks5ProxiesParameter)) {
        d->ui->proxyGroupBox->hide();
    }

    if (!d->fallbackConferenceServerParameter) {
        d->ui->conferenceGroupBox->hide();
    }
}

ProxySettingsWidget::~ProxySettingsWidget()
{
    kDebug();

    delete d;
}

QMap<Tp::ProtocolParameter*, QVariant> ProxySettingsWidget::parameterValues() const
{
    kDebug();

    QMap<Tp::ProtocolParameter*, QVariant> parameters;

    // Populate the map of parameters and their values with all the parameters this widget contains.
    if (d->stunServerParameter) {
        parameters.insert(d->stunServerParameter, d->ui->stunServerLineEdit->text());
    }

    if (d->stunPortParameter) {
        parameters.insert(d->stunPortParameter, d->ui->stunPortLineEdit->value());
    }

    if (d->fallbackStunServerParameter) {
        parameters.insert(d->fallbackStunServerParameter,
                          d->ui->fallbackStunServerLineEdit->text());
    }

    if (d->fallbackStunPortParameter) {
        parameters.insert(d->fallbackStunPortParameter, d->ui->fallbackStunPortLineEdit->value());
    }

    if (d->httpsProxyServerParameter) {
        parameters.insert(d->httpsProxyServerParameter, d->ui->httpsProxyServerLineEdit->text());
    }

    if (d->httpsProxyPortParameter) {
        parameters.insert(d->httpsProxyPortParameter, d->ui->httpsProxyPortLineEdit->value());
    }

    if (d->fallbackSocks5ProxiesParameter) {
        QString text = d->ui->fallbackSocks5ProxiesTextEdit->toPlainText();
        QStringList value = text.split("\n", QString::SkipEmptyParts);
        parameters.insert(d->fallbackSocks5ProxiesParameter, value);
    }

    if (d->fallbackConferenceServerParameter) {
        parameters.insert(d->fallbackConferenceServerParameter,
                          d->ui->fallbackConferenceServerLineEdit->text());
    }

    return parameters;
}

bool ProxySettingsWidget::validateParameterValues()
{
    kDebug();

    return true;
}


#include "proxy-settings-widget.moc"

