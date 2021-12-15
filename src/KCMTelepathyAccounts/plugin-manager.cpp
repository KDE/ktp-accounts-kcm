/*
 * This file is part of Kopete
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

#include "plugin-manager.h"

#include "abstract-account-ui-plugin.h"
#include "abstract-account-ui.h"

#include <KServiceTypeTrader>
#include <QDebug>

PluginManager* PluginManager::s_self = nullptr;

PluginManager::PluginManager(QObject *parent)
 : QObject(parent)
{
    // Set up the singleton instance
    s_self = this;

    loadPlugins();
}

PluginManager::~PluginManager()
{
    // Delete the singleton instance of this class
    s_self = nullptr;
}

PluginManager *PluginManager::instance()
{
    // Construct the singleton if hasn't been already
    if (!s_self) {
        s_self = new PluginManager(nullptr);
    }

    // Return the singleton instance of this class
    return s_self;
}

void PluginManager::loadPlugins()
{
    KService::List offers = KServiceTypeTrader::self()->query(QLatin1String("KTpAccountsKCM/AccountUiPlugin"));

    KService::List::const_iterator iter;
    for (iter = offers.constBegin(); iter < offers.constEnd(); ++iter) {
       QString error;
       KService::Ptr service = *iter;

        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory) {
            qWarning() << "KPluginFactory could not load the plugin:" << service->library();
            continue;
        }

       AbstractAccountUiPlugin *plugin = factory->create<AbstractAccountUiPlugin>(this);

       if (plugin) {
           qDebug() << "Loaded plugin:" << service->name();
           m_plugins.append(plugin);
       } else {
           qDebug() << error;
       }
    }
}

AbstractAccountUi *PluginManager::accountUiForProtocol(const QString &connectionManager,
                                                       const QString &protocol,
                                                       const QString &serviceName)
{
    // Loop through all the plugins seeing if they provide an AccountUi for the connection manager
    // and protocol combination we were provided with.

    Q_FOREACH (AbstractAccountUiPlugin *plugin, m_plugins) {
        AbstractAccountUi *ui = plugin->accountUi(connectionManager, protocol, serviceName);

        // FIXME: Bug https://bugs.kde.org/201797 - we should check here to see which plugin
        // provides the closest match for the desired parameters in the case that more than one
        // plugin provides a UI for this protocol/cm pair.
        if (ui) {
            return ui;
        }
    }

    return nullptr;
}
