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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_UI_PLUGIN_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_UI_PLUGIN_H

#include <kdemacros.h>

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>

class AbstractAccountUi;

class KDE_EXPORT AbstractAccountUiPlugin : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractAccountUiPlugin);

public:
    AbstractAccountUiPlugin(QObject *parent = 0);
    virtual ~AbstractAccountUiPlugin();

    virtual const QMap<QString, QString> &providedProtocols() const;

    virtual AbstractAccountUi* accountUi(const QString &connectionManager, const QString &protocol) = 0;

protected:
    virtual void registerProvidedProtocol(const QString &connectionManager, const QString &protocol);

private:
    class Private;
    Private * const d;
};


#endif // header guard

