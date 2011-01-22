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
            : ui(0)
    {
        kDebug();
    }

    Ui::ProxySettingsWidget *ui;
};

ProxySettingsWidget::ProxySettingsWidget(ParameterEditModel *model,
                                         QWidget *parent)
 : AbstractAccountParametersWidget(model, parent),
   d(new Private)
{
    kDebug();

    // Set up the UI.
    d->ui = new Ui::ProxySettingsWidget;
    d->ui->setupUi(this);

    handleParameter("stun-server", QVariant::String, d->ui->stunServerLineEdit, d->ui->stunServerLabel);
    handleParameter("stun-port", QVariant::UInt, d->ui->stunPortSpinBox, d->ui->stunPortLabel);
    handleParameter("fallback-stun-server", QVariant::String, d->ui->fallbackStunServerLineEdit, d->ui->fallbackStunServerLabel);
    handleParameter("fallback-stun-port", QVariant::UInt, d->ui->fallbackStunPortSpinBox, d->ui->fallbackStunPortLabel);
    handleParameter("https-proxy-server", QVariant::String, d->ui->httpsProxyServerLineEdit, d->ui->httpsProxyServerLabel);
    handleParameter("https-proxy-port", QVariant::UInt, d->ui->httpsProxyPortSpinBox, d->ui->httpsProxyPortLabel);
    handleParameter("fallback-socks5-proxies", QVariant::StringList, d->ui->fallbackSocks5ProxiesTextEdit, d->ui->fallbackSocks5ProxiesLabel);
    handleParameter("fallback-conference-server", QVariant::String, d->ui->fallbackConferenceServerLineEdit, d->ui->fallbackConferenceServerLabel);
}

ProxySettingsWidget::~ProxySettingsWidget()
{
    kDebug();

    delete d;
}


#include "proxy-settings-widget.moc"

