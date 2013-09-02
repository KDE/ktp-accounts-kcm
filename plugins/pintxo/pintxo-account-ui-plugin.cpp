/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2013 Anant Kamath <kamathanant@gmail.com>
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

#include "pintxo-account-ui-plugin.h"

#include "pintxo-account-ui.h"

#include <KPluginFactory>

#include <QtCore/QVariantList>

PintxoAccountUiPlugin::PintxoAccountUiPlugin(QObject *parent, const QVariantList &)
 : AbstractAccountUiPlugin(parent)
{
    // Register supported cm/protocol combinations
    registerProvidedProtocol(QLatin1String("pintxo"), QLatin1String("sms"));
}

PintxoAccountUiPlugin::~PintxoAccountUiPlugin()
{
}

AbstractAccountUi* PintxoAccountUiPlugin::accountUi(const QString &connectionManager, const QString &protocol, const QString &serviceName)
{
    Q_UNUSED(serviceName);

    // We only support pintxo/sms combination.
    if ((connectionManager == QLatin1String("pintxo")) && (protocol == QLatin1String("sms"))) {
        return new PintxoAccountUi;
    }

    return 0;
}

K_PLUGIN_FACTORY(factory, registerPlugin<PintxoAccountUiPlugin>();)
K_EXPORT_PLUGIN(factory("kcmtelepathyaccounts_plugin_pintxo"))

#include "pintxo-account-ui-plugin.moc"
