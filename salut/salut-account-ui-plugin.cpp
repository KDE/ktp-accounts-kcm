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

#include "salut-account-ui-plugin.h"
#include "salut-account-ui.h"

#include <KCMTelepathyAccounts/PluginMacros>

#include <KDebug>
#include <KGenericFactory>

#include <QtCore/QVariantList>

SalutAccountUiPlugin::SalutAccountUiPlugin(QObject *parent, const QVariantList &)
    : AbstractAccountUiPlugin(parent)
{
    kDebug();

    // Register supported cm/protocol combinations
    registerProvidedProtocol("salut", "local-xmpp");
}

SalutAccountUiPlugin::~SalutAccountUiPlugin()
{
    kDebug();
}

AbstractAccountUi* SalutAccountUiPlugin::accountUi(const QString &connectionManager,
        const QString &protocol,
        const QString &serviceName)
{
    kDebug();
    kDebug() << "connectionManager: " << connectionManager <<
             ", protocol: " << protocol <<
             ", servicename: " << serviceName;

    // We support salut/local-xmpp
    if(connectionManager == QLatin1String("salut") && protocol == QLatin1String("salut"))
    {
        return new SalutAccountUi;
    }

    return 0;
}

K_PLUGIN_FACTORY(factory, registerPlugin<SalutAccountUiPlugin>();)
K_EXPORT_PLUGIN(factory("kcmtelepathyaccounts_plugin_salut"))

#include "salut-account-ui-plugin.moc"
