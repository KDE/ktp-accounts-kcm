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

#include "gabble-account-ui.h"

#include "main-options-widget.h"
#include "server-settings-widget.h"
#include "proxy-settings-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>
#include <KCMTelepathyAccounts/GenericAdvancedOptionsWidget>

#include <KDebug>

class GabbleAccountUi::Private
{
public:
    Private()
    {
        kDebug();
    }
};

GabbleAccountUi::GabbleAccountUi(QObject *parent)
 : AbstractAccountUi(parent),
   d(new Private)
{
    kDebug();

    // Register supported parameters
    registerSupportedParameter("account", QVariant::String);
    registerSupportedParameter("password", QVariant::String);

    registerSupportedParameter("port", QVariant::UInt);
    registerSupportedParameter("server", QVariant::String);
    registerSupportedParameter("require-encryption", QVariant::Bool);
    registerSupportedParameter("old-ssl", QVariant::Bool);
    registerSupportedParameter("low-bandwidth", QVariant::Bool);
    registerSupportedParameter("ignore-ssl-errors", QVariant::Bool);
    registerSupportedParameter("keepalive-interval", QVariant::UInt);

    registerSupportedParameter("stun-server", QVariant::String);
    registerSupportedParameter("stun-port", QVariant::UInt);
    registerSupportedParameter("fallback-stun-server", QVariant::String);
    registerSupportedParameter("fallback-stun-port", QVariant::UInt);
    registerSupportedParameter("https-proxy-server", QVariant::String);
    registerSupportedParameter("https-proxy-port", QVariant::UInt);
    registerSupportedParameter("fallback-socks5-proxies", QVariant::StringList);
    registerSupportedParameter("fallback-conference-server", QVariant::String);
}

GabbleAccountUi::~GabbleAccountUi()
{
    kDebug();

    delete d;
}

AbstractAccountParametersWidget *GabbleAccountUi::mainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{

    return new MainOptionsWidget(model, parent);
}

bool GabbleAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *GabbleAccountUi::advancedOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    kDebug();

    GenericAdvancedOptionsWidget *widget = new GenericAdvancedOptionsWidget(model, parent);

    AbstractAccountParametersWidget* serverSettingsWidget = new ServerSettingsWidget(model, widget);
    widget->addTab(serverSettingsWidget, "Server");
    AbstractAccountParametersWidget* proxySettingsWidget = new ProxySettingsWidget(model, widget);
    widget->addTab(proxySettingsWidget, "Proxy");

    return widget;
}


#include "gabble-account-ui.moc"

