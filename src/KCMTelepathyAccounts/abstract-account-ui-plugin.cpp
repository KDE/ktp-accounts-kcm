/*
 * This file is part of telepathy-accounts-kcm
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

#include "abstract-account-ui-plugin.h"

#include <KDebug>

class AbstractAccountUiPlugin::Private
{
public:
    Private()
    {
    }

    QMap<QString, QString> providedProtocols;
};


AbstractAccountUiPlugin::AbstractAccountUiPlugin(QObject *parent)
 : QObject(parent),
   d(new Private)
{
}

AbstractAccountUiPlugin::~AbstractAccountUiPlugin()
{
    delete d;
}

const QMap<QString, QString> &AbstractAccountUiPlugin::providedProtocols() const
{
    return d->providedProtocols;
}

void AbstractAccountUiPlugin::registerProvidedProtocol(const QString &connectionManager,
                                                       const QString &protocol)
{
    // Check the protocol is not already entered
    foreach (const QString &value, d->providedProtocols.values(connectionManager)) {
        if (value == protocol) {
            kWarning() << "Tried to add connection manager:" << connectionManager
                       << "and protocol:" << protocol << "combination twice.";
            return;
        }
    }

    // Pair is not already added. Add it.
    d->providedProtocols.insertMulti(connectionManager, protocol);
}


#include "abstract-account-ui-plugin.moc"
