/*
 * This file is part of telepathy-accounts-kcm-plugins
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
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

#include "rakia-account-ui.h"
#include "rakia-main-options-widget.h"
#include "rakia-advanced-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>
#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

RakiaAccountUi::RakiaAccountUi(QObject *parent)
    : AbstractAccountUi(parent)
{
    // Register supported parameters
    // Main Options
    /**< public SIP address (SIP URI) */
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    /**< account password (for registration) */
    registerSupportedParameter(QLatin1String("password"), QVariant::String);
    /**< Display name for self */
    registerSupportedParameter(QLatin1String("alias"), QVariant::String);

    // Advanced Options
    /**< account username (if different from public address userinfo part) */
    registerSupportedParameter(QLatin1String("auth-user"), QVariant::String);
    /**< SIP registrar (SIP URI) */
    registerSupportedParameter(QLatin1String("registrar"), QVariant::String);

    // Parameters being used to compose the proxy uri
    /**< outbound SIP proxy (SIP URI) */
    registerSupportedParameter(QLatin1String("proxy-host"), QVariant::String);
    registerSupportedParameter(QLatin1String("port"), QVariant::UInt);
    /**< outbound transport */
    registerSupportedParameter(QLatin1String("transport"), QVariant::String);

    /**< Discover STUN server name using DNS SRV lookup */
    registerSupportedParameter(QLatin1String("discover-stun"), QVariant::Bool);
    /**< STUN server address (if not set, derived from public SIP address) */
    registerSupportedParameter(QLatin1String("stun-server"), QVariant::String);
    /**< STUN port */
    registerSupportedParameter(QLatin1String("stun-port"), QVariant::UInt);

    /**< enable discovery of public binding */
    registerSupportedParameter(QLatin1String("discover-binding"), QVariant::Bool);
    /**< enable loose routing behavior */
    registerSupportedParameter(QLatin1String("loose-routing"), QVariant::Bool);
    /**< keepalive mechanism as defined by TpsipConnectionKeepaliveMechanism
     * "auto","register","options","stun","off" */
    registerSupportedParameter(QLatin1String("keepalive-mechanism"), QVariant::String);
    /**< keepalive interval in seconds */
    registerSupportedParameter(QLatin1String("keepalive-interval"), QVariant::UInt);

//  Parameters, we know about their existence but don't expose them to the user
//  for eventual later usage, we keep them documented here:

//  immuteable-streams: if set to True, you can't add video to an already running voice call.
//     /**< If the session content is immutable once set up */
//     registerSupportedParameter(QLatin1String("immutable-streams"), QVariant::Bool);

//  local-ip-addres, local-port:
//  These affect the socket binding of the SIP UA. It's a way to ensure that Sofia-SIP uses a
//  particular local IP binding on multi-homed hosts, something that it does not do in a
//  satisfactory way automatically.
//     /**< Local IP address (normally not needed, chosen by stack) */
//     registerSupportedParameter(QLatin1String("local-ip-address"), QVariant::String);
//     /**< Local port for SIP (normally not needed, chosen by stack) */
//     registerSupportedParameter(QLatin1String("local-port"), QVariant::UInt);

//  extra-auth-user, extra-auth-password:
//  ifdigest authentication on demand is implemented, these parameters are obsolete.
//     /**< User name to use for extra authentication challenges */
//     registerSupportedParameter(QLatin1String("extra-auth-user"), QVariant::String);
//     /**< Password to use for extra authentication challenges */
//     registerSupportedParameter(QLatin1String("extra-auth-password"), QVariant::String);
}

RakiaAccountUi::~RakiaAccountUi()
{
}

AbstractAccountParametersWidget *RakiaAccountUi::mainOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    return new RakiaMainOptionsWidget(model, parent);
}

bool RakiaAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *RakiaAccountUi::advancedOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    AbstractAccountParametersWidget* aowidget = new RakiaAdvancedOptionsWidget(model, parent);

    return aowidget;
}

#include "rakia-account-ui.moc"
