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

#include "gabble-account-ui.h"

#include "main-options-widget.h"
#include "main-options-widget-googletalk.h"
#include "main-options-widget-facebook.h"
#include "main-options-widget-msn.h"
#include "main-options-widget-kde-talk.h"
#include "server-settings-widget.h"
#include "proxy-settings-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>
#include <KCMTelepathyAccounts/GenericAdvancedOptionsWidget>


GabbleAccountUi::GabbleAccountUi(const QString &serviceName, QObject *parent)
 : AbstractAccountUi(parent),
   m_serviceName(serviceName)
{
    // Register supported parameters
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);

    registerSupportedParameter(QLatin1String("port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("server"), QVariant::String);
    registerSupportedParameter(QLatin1String("require-encryption"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("old-ssl"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("low-bandwidth"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("ignore-ssl-errors"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("keepalive-interval"), QVariant::UInt);

    registerSupportedParameter(QLatin1String("stun-server"), QVariant::String);
    registerSupportedParameter(QLatin1String("stun-port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("fallback-stun-server"), QVariant::String);
    registerSupportedParameter(QLatin1String("fallback-stun-port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("https-proxy-server"), QVariant::String);
    registerSupportedParameter(QLatin1String("https-proxy-port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("fallback-socks5-proxies"), QVariant::StringList);
    registerSupportedParameter(QLatin1String("fallback-conference-server"), QVariant::String);
}

GabbleAccountUi::~GabbleAccountUi()
{
}

AbstractAccountParametersWidget *GabbleAccountUi::mainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    if (m_serviceName == QLatin1String("google-talk")) {
        return new MainOptionsWidgetGoogleTalk(model, parent);
    } else if (m_serviceName == QLatin1String("facebook")) {
        return new MainOptionsWidgetFacebook(model, parent);
    } else if (m_serviceName == QLatin1String("msn-xmpp")) {
        return new MainOptionsWidgetMSN(model, parent);
    } else if (m_serviceName == QLatin1String("kde-talk")) {
        return new MainOptionsWidgetKDETalk(model, parent);
    } else {
        return new MainOptionsWidget(model, parent);
    }
}

bool GabbleAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *GabbleAccountUi::advancedOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    GenericAdvancedOptionsWidget *widget = new GenericAdvancedOptionsWidget(model, parent);

    AbstractAccountParametersWidget* serverSettingsWidget = new ServerSettingsWidget(model, widget);
    widget->addTab(serverSettingsWidget, i18n("Server"));
    AbstractAccountParametersWidget* proxySettingsWidget = new ProxySettingsWidget(model, widget);
    widget->addTab(proxySettingsWidget, i18n("Proxy"));

    return widget;
}


#include "gabble-account-ui.moc"
