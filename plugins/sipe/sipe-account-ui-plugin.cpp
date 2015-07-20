/*
 * This file is part of telepathy-accounts-kcm-plugins
 *
 * Copyright (C) 2013 Maksim Melnikau <maxposedon@gmail.com>
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

#include "sipe-account-ui-plugin.h"
#include "sipe-account-ui.h"

#include <KPluginFactory>

#include <QtCore/QVariantList>

SipeAccountUiPlugin::SipeAccountUiPlugin(QObject *parent, const QVariantList &)
 : AbstractAccountUiPlugin(parent)
{
    registerProvidedProtocol(QLatin1String("sipe"), QLatin1String("sipe"));
}

SipeAccountUiPlugin::~SipeAccountUiPlugin()
{
}

AbstractAccountUi* SipeAccountUiPlugin::accountUi(
    const QString &connectionManager,
    const QString &protocol,
    const QString &serviceName)
{
    Q_UNUSED(serviceName);

    if (connectionManager == QLatin1String("sipe") && protocol == QLatin1String("sipe")) {
        AbstractAccountUi *sipeAccountUi = new SipeAccountUi(this);
        return sipeAccountUi;
    }
    return 0;
}

K_PLUGIN_FACTORY(factory, registerPlugin<SipeAccountUiPlugin>();)

#include "sipe-account-ui-plugin.moc"
