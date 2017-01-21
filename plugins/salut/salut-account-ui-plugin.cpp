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

#include <KPluginFactory>

#include <QtCore/QVariantList>

SalutAccountUiPlugin::SalutAccountUiPlugin(QObject *parent, const QVariantList &)
    : AbstractAccountUiPlugin(parent)
{
    // Register supported cm/protocol combinations
    registerProvidedProtocol(QLatin1String("salut"), QLatin1String("local-xmpp"));
}

SalutAccountUiPlugin::~SalutAccountUiPlugin()
{
}

AbstractAccountUi* SalutAccountUiPlugin::accountUi(const QString &connectionManager,
        const QString &protocol,
        const QString &serviceName)
{
    // We support salut/local-xmpp
    if (connectionManager == QLatin1String("salut") && protocol == QLatin1String("local-xmpp")) {
        return new SalutAccountUi;
    }

    return 0;
}

K_PLUGIN_FACTORY(factory, registerPlugin<SalutAccountUiPlugin>();)

#include "salut-account-ui-plugin.moc"
