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

#include "mandatory-parameters-widget.h"
#include "proxy-settings-widget.h"
#include "server-settings-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

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
    registerSupportedMandatoryParameter("account", QVariant::String);
    registerSupportedMandatoryParameter("password", QVariant::String);

    registerSupportedOptionalParameter("port", QVariant::UInt);
    registerSupportedOptionalParameter("server", QVariant::String);
    registerSupportedOptionalParameter("require-encryption", QVariant::Bool);
    registerSupportedOptionalParameter("old-ssl", QVariant::Bool);
    registerSupportedOptionalParameter("low-bandwidth", QVariant::Bool);
    registerSupportedOptionalParameter("ignore-ssl-errors", QVariant::Bool);
    registerSupportedOptionalParameter("keepalive-interval", QVariant::UInt);

    registerSupportedOptionalParameter("stun-server", QVariant::String);
    registerSupportedOptionalParameter("stun-port", QVariant::UInt);
    registerSupportedOptionalParameter("fallback-stun-server", QVariant::String);
    registerSupportedOptionalParameter("fallback-stun-port", QVariant::UInt);
    registerSupportedOptionalParameter("https-proxy-server", QVariant::String);
    registerSupportedOptionalParameter("https-proxy-port", QVariant::UInt);
    registerSupportedOptionalParameter("fallback-socks5-proxies", QVariant::StringList);
    registerSupportedOptionalParameter("fallback-conference-server", QVariant::String);
}

GabbleAccountUi::~GabbleAccountUi()
{
    kDebug();

    delete d;
}

AbstractAccountParametersWidget *GabbleAccountUi::mandatoryParametersWidget(
        Tp::ProtocolParameterList parameters,
        const QVariantMap &values) const
{
    kDebug();

    return new MandatoryParametersWidget(parameters, values);
}

QList<AbstractAccountParametersWidget*> GabbleAccountUi::optionalParametersWidgets(
        Tp::ProtocolParameterList parameters,
        const QVariantMap &values) const
{
    kDebug();

    QList<AbstractAccountParametersWidget*> widgets;

    // Add each of the optional parameter widgets.
    widgets.append(new ServerSettingsWidget(parameters, values));
    widgets.append(new ProxySettingsWidget(parameters, values));

    return widgets;
}


#include "gabble-account-ui.moc"

