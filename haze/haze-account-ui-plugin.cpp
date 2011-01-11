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

#include "haze-account-ui-plugin.h"

#include "haze-icq-account-ui.h"

#include <KCMTelepathyAccounts/PluginMacros>

#include <KDebug>
#include <KGenericFactory>

#include <QtCore/QVariantList>

class HazeAccountUiPlugin::Private
{
public:
    Private()
    {
        kDebug();
    }
};

HazeAccountUiPlugin::HazeAccountUiPlugin(QObject *parent, const QVariantList &)
 : AbstractAccountUiPlugin(parent),
   d(new Private)
{
    kDebug();

    // Register supported cm/protocol combinations
    registerProvidedProtocol("haze", "icq");
}

HazeAccountUiPlugin::~HazeAccountUiPlugin()
{
    kDebug();
}

AbstractAccountUi* HazeAccountUiPlugin::accountUi(const QString &connectionManager, const QString &protocol)
{
    kDebug();

    // We only support haze/icq combination.so far
    if ((connectionManager == "haze") && (protocol == "icq")) {
        return new HazeIcqAccountUi;
    }

    return 0;
}


//KCMTELEPATHYACCOUNTS_PLUGIN_EXPORT("haze", "HazeAccountUiPlugin")
  K_PLUGIN_FACTORY(factory, registerPlugin<HazeAccountUiPlugin>();) \
  K_EXPORT_PLUGIN(factory("kcmtelepathyaccounts_plugin_haze"))


#include "haze-account-ui-plugin.moc"

