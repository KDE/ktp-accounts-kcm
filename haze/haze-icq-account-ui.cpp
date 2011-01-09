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

#include "haze-icq-account-ui.h"

#include "icq-main-options-widget.h"
#include "icq-advanced-options-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

#include <KDebug>

class HazeIcqAccountUi::Private
{
public:
    Private()
    {
        kDebug();
    }
};

HazeIcqAccountUi::HazeIcqAccountUi(QObject *parent)
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

HazeIcqAccountUi::~HazeIcqAccountUi()
{
    kDebug();

    delete d;
}

AbstractAccountParametersWidget *HazeIcqAccountUi::mainOptionsWidget(
        Tp::ProtocolParameterList parameters,
        const QVariantMap &values,
        QWidget *parent) const
{
    kDebug();

    return new IcqMainOptionsWidget(parameters, values, parent);
}

bool HazeIcqAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *HazeIcqAccountUi::advancedOptionsWidget(
        Tp::ProtocolParameterList parameters,
        const QVariantMap &values,
        QWidget *parent) const
{
    kDebug();

    return new IcqAdvancedOptionsWidget(parameters, values, parent);
}


#include "haze-icq-account-ui.moc"

