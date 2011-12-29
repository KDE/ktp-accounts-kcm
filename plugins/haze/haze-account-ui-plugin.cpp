/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#include "haze-icq-account.h"
#include "haze-myspaceim-account-ui.h"
#include "haze-skype-account-ui.h"
#include "haze-yahoo-account.h"
#include "haze-aim-account.h"
#include "haze-msn-account.h"

#include <KGenericFactory>

#include <QtCore/QVariantList>

HazeAccountUiPlugin::HazeAccountUiPlugin(QObject *parent, const QVariantList &)
 : AbstractAccountUiPlugin(parent)
{
    // Register supported cm/protocol combinations
    registerProvidedProtocol("haze", "icq");
    registerProvidedProtocol("haze", "myspace");
    registerProvidedProtocol("haze", "bigbrownchunx-skype-dbus");
    registerProvidedProtocol("haze", "yahoo");
    registerProvidedProtocol("haze", "aim");
    registerProvidedProtocol("haze", "msn");
}

HazeAccountUiPlugin::~HazeAccountUiPlugin()
{
}

AbstractAccountUi* HazeAccountUiPlugin::accountUi(const QString &connectionManager, const QString &protocol, const QString &serviceName)
{
    Q_UNUSED(serviceName);

    if (connectionManager == QLatin1String("haze")) {
        if (protocol == QLatin1String("icq")) {
            return new HazeIcqAccountUi;
        } else if (protocol == QLatin1String("myspace")) {
            return new HazeMySpaceIMAccountUi;
        } else if (protocol == QLatin1String("bigbrownchunx-skype-dbus")) {
            return new HazeSkypeAccountUi;
        } else if (protocol == QLatin1String("yahoo")) {
            return new HazeYahooAccount;
        } else if (protocol == QLatin1String("aim")) {
            return new HazeAimAccount;
        } else if (protocol == QLatin1String("msn")) {
            return new HazeMsnAccountUi;
        }
    }

    return 0;
}


K_PLUGIN_FACTORY(factory, registerPlugin<HazeAccountUiPlugin>();)
K_EXPORT_PLUGIN(factory("kcmtelepathyaccounts_plugin_haze"))

#include "haze-account-ui-plugin.moc"
