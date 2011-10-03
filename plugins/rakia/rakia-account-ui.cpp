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

#include <KDebug>

RakiaAccountUi::RakiaAccountUi(QObject *parent)
    : AbstractAccountUi(parent)
{
    kDebug();
    // Register supported parameters
    // Main Options
    /**< public SIP address (SIP URI) */
    registerSupportedParameter("account", QVariant::String);
    /**< account password (for registration) */
    registerSupportedParameter("password", QVariant::String);
    /**< Display name for self */
    registerSupportedParameter("alias", QVariant::String);

    // Advanced Options
    /**< account username (if different from public address userinfo part) */
    registerSupportedParameter("auth-user", QVariant::String);
    /**< SIP registrar (SIP URI) */
    registerSupportedParameter("registrar", QVariant::String);

    // Parameters being used to compose the proxy uri
    /**< outbound SIP proxy (SIP URI) */
    registerSupportedParameter("proxy-host", QVariant::String);
    registerSupportedParameter("port", QVariant::UInt);
    /**< outbound transport */
    registerSupportedParameter("transport", QVariant::String);

    /**< Discover STUN server name using DNS SRV lookup */
    registerSupportedParameter("discover-stun", QVariant::Bool);
    /**< STUN server address (if not set, derived from public SIP address) */
    registerSupportedParameter("stun-server", QVariant::String);
    /**< STUN port */
    registerSupportedParameter("stun-port", QVariant::UInt);

    /**< enable discovery of public binding */
    registerSupportedParameter("discover-binding", QVariant::Bool);
    /**< enable loose routing behavior */
    registerSupportedParameter("loose-routing", QVariant::Bool);
    /**< keepalive mechanism as defined by TpsipConnectionKeepaliveMechanism
     * "auto","register","options","stun","off" */
    registerSupportedParameter("keepalive-mechanism", QVariant::String);
    /**< keepalive interval in seconds */
    registerSupportedParameter("keepalive-interval", QVariant::UInt);

//  Parameters, we know about their existance but don't expose them to the user
//  for eventual later usage, we keep them documented here:

//  immuteable-streams: if set to True, you can't add video to an already running voice call.
//     /**< If the session content is immutable once set up */
//     registerSupportedParameter("immutable-streams", QVariant::Bool);

//  local-ip-addres, local-port:
//  These affect the socket binding of the SIP UA. It's a way to ensure that Sofia-SIP uses a
//  particular local IP binding on multi-homed hosts, something that it does not do in a
//  satisfactory way automatically.
//     /**< Local IP address (normally not needed, chosen by stack) */
//     registerSupportedParameter("local-ip-address", QVariant::String);
//     /**< Local port for SIP (normally not needed, chosen by stack) */
//     registerSupportedParameter("local-port", QVariant::UInt);

//  extra-auth-user, extra-auth-password:
//  ifdigest authentication on demand is implemented, these parameters are obsolete.
//     /**< User name to use for extra authentication challenges */
//     registerSupportedParameter("extra-auth-user", QVariant::String);
//     /**< Password to use for extra authentication challenges */
//     registerSupportedParameter("extra-auth-password", QVariant::String);
}

RakiaAccountUi::~RakiaAccountUi()
{
    kDebug();
}

AbstractAccountParametersWidget *RakiaAccountUi::mainOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    kDebug();

    return new RakiaMainOptionsWidget(model, parent);
}

bool RakiaAccountUi::hasAdvancedOptionsWidget() const
{
    kDebug();

    return true;
}

AbstractAccountParametersWidget *RakiaAccountUi::advancedOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    kDebug();

    AbstractAccountParametersWidget* aowidget = new RakiaAdvancedOptionsWidget(model, parent);

    return aowidget;
}

#include "rakia-account-ui.moc"
