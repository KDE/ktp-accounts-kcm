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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_PINTXO_ACCOUNT_UI_PLUGIN_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_PINTXO_ACCOUNT_UI_PLUGIN_H

#include <KCMTelepathyAccounts/AbstractAccountUiPlugin>

#include <QtCore/QVariantList>

class PintxoAccountUiPlugin : public AbstractAccountUiPlugin
{
    Q_OBJECT

public:
    PintxoAccountUiPlugin(QObject *parent, const QVariantList &);
    virtual ~PintxoAccountUiPlugin();

    virtual AbstractAccountUi* accountUi(const QString &connectionManager, const QString &protocol, const QString &serviceName);

private:
    Q_DISABLE_COPY(PintxoAccountUiPlugin);
};


#endif // header guard

