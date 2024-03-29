/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

ProxySettingsWidget::ProxySettingsWidget(ParameterEditModel *model,
                                         QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::ProxySettingsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("stun-server"), QVariant::String, m_ui->stunServerLineEdit, m_ui->stunServerLabel);
    handleParameter(QLatin1String("stun-port"), QVariant::UInt, m_ui->stunPortSpinBox, m_ui->stunPortLabel);
    handleParameter(QLatin1String("fallback-stun-server"), QVariant::String, m_ui->fallbackStunServerLineEdit, m_ui->fallbackStunServerLabel);
    handleParameter(QLatin1String("fallback-stun-port"), QVariant::UInt, m_ui->fallbackStunPortSpinBox, m_ui->fallbackStunPortLabel);
    handleParameter(QLatin1String("https-proxy-server"), QVariant::String, m_ui->httpsProxyServerLineEdit, m_ui->httpsProxyServerLabel);
    handleParameter(QLatin1String("https-proxy-port"), QVariant::UInt, m_ui->httpsProxyPortSpinBox, m_ui->httpsProxyPortLabel);
    handleParameter(QLatin1String("fallback-socks5-proxies"), QVariant::StringList, m_ui->fallbackSocks5ProxiesTextEdit, m_ui->fallbackSocks5ProxiesLabel);
    handleParameter(QLatin1String("fallback-conference-server"), QVariant::String, m_ui->fallbackConferenceServerLineEdit, m_ui->fallbackConferenceServerLabel);
}

ProxySettingsWidget::~ProxySettingsWidget()
{
    delete m_ui;
}
