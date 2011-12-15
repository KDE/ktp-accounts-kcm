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

#include <KDebug>

ProxySettingsWidget::ProxySettingsWidget(ParameterEditModel *model,
                                         QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    kDebug();

    // Set up the UI.
    m_ui = new Ui::ProxySettingsWidget;
    m_ui->setupUi(this);

    handleParameter("stun-server", QVariant::String, m_ui->stunServerLineEdit, m_ui->stunServerLabel);
    handleParameter("stun-port", QVariant::UInt, m_ui->stunPortSpinBox, m_ui->stunPortLabel);
    handleParameter("fallback-stun-server", QVariant::String, m_ui->fallbackStunServerLineEdit, m_ui->fallbackStunServerLabel);
    handleParameter("fallback-stun-port", QVariant::UInt, m_ui->fallbackStunPortSpinBox, m_ui->fallbackStunPortLabel);
    handleParameter("https-proxy-server", QVariant::String, m_ui->httpsProxyServerLineEdit, m_ui->httpsProxyServerLabel);
    handleParameter("https-proxy-port", QVariant::UInt, m_ui->httpsProxyPortSpinBox, m_ui->httpsProxyPortLabel);
    handleParameter("fallback-socks5-proxies", QVariant::StringList, m_ui->fallbackSocks5ProxiesTextEdit, m_ui->fallbackSocks5ProxiesLabel);
    handleParameter("fallback-conference-server", QVariant::String, m_ui->fallbackConferenceServerLineEdit, m_ui->fallbackConferenceServerLabel);
}

ProxySettingsWidget::~ProxySettingsWidget()
{
    kDebug();

    delete m_ui;
}


#include "proxy-settings-widget.moc"

