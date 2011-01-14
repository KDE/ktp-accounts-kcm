/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
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

#ifndef TELEPATHY_ACCOUNTS_KCM_PROTOCOL_ITEM_H
#define TELEPATHY_ACCOUNTS_KCM_PROTOCOL_ITEM_H

#include <kdemacros.h>

#include <QtCore/QObject>

#include <TelepathyQt4/Profile>

class ProfileListModel;

class KDE_EXPORT ProfileItem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ProfileItem);

public:
    explicit ProfileItem(const Tp::ProfilePtr &profile, ProfileListModel *parent);
    virtual ~ProfileItem();

    QString serviceName() const;
    QString name() const;
    QString localizedName() const;
    QString iconName() const;
    QString cmName() const;
    QString protocolName() const;
    Tp::ProfilePtr profile() const;

private:
    Tp::ProfilePtr m_profile;
    QString m_localizedName;
};


#endif // header guard

