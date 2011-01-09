/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Lasse Liehu <lliehu@kolumbus.fi>
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

#include "idle-account-ui-plugin.h"

#include "idle-account-ui.h"

#include <KCMTelepathyAccounts/PluginMacros>

#include <KDebug>
#include <KGenericFactory>

#include <QtCore/QVariantList>

#include <KCMTelepathyAccounts/ProtocolParameterValue>

class IdleAccountUiPlugin::Private
{
public:
    Private()
    {
        kDebug();
    }
};

IdleAccountUiPlugin::IdleAccountUiPlugin(QObject *parent, const QVariantList &)
 : AbstractAccountUiPlugin(parent),
   d(new Private)
{
    kDebug();

    // Register supported cm/protocol combinations
    registerProvidedProtocol("idle", "irc");
}

IdleAccountUiPlugin::~IdleAccountUiPlugin()
{
    kDebug();
}

AbstractAccountUi* IdleAccountUiPlugin::accountUi(const QString &connectionManager, const QString &protocol)
{
    kDebug();
    
    // We only support butterfly/xmpp combination.
    if ((connectionManager == "idle") && (protocol == "irc")) {
        return new IdleAccountUi;
    }

    return 0;
}


//KCMTELEPATHYACCOUNTS_PLUGIN_EXPORT("idle", "IdleAccountUiPlugin")
  K_PLUGIN_FACTORY(factory, registerPlugin<IdleAccountUiPlugin>();) \
  K_EXPORT_PLUGIN(factory("kcmtelepathyaccounts_plugin_idle"))


#include "idle-account-ui-plugin.moc"

