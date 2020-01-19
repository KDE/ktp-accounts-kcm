/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_PROFILE_LIST_MODEL_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_PROFILE_LIST_MODEL_H

#include "kcm_telepathy_accounts_export.h"

#include <QAbstractListModel>

#include <TelepathyQt/ConnectionManager>

namespace TelepathyQt {
    class PendingOperation;
}

class ProfileItem;

class KCM_TELEPATHY_ACCOUNTS_EXPORT ProfileListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ProfileListModel)

public:
    enum Roles {
        ProfileProtocolNameRole = Qt::UserRole+1,
        ProfileCmNameRole = Qt::UserRole+2
    };

    explicit ProfileListModel(QObject *parent = 0);
    ~ProfileListModel() override;

    int rowCount(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    ProfileItem *itemForIndex(const QModelIndex &index) const;
    ProfileItem *itemForService(const QString &serviceName) const;

private Q_SLOTS:
    void onConnectionManagerNamesFetched(Tp::PendingOperation*);
    void onConnectionManagersLoaded(Tp::PendingOperation *op);
    void onProfileManagerReady(Tp::PendingOperation *op);

private:
    bool hasNonFakeProfile(const Tp::ProfilePtr &profile, const QList<Tp::ProfilePtr> &profiles) const;
    bool hasValidCM(const Tp::ProfilePtr &profile) const;
    void populateList();

    Tp::ProfileManagerPtr m_profileManager;
    QList<ProfileItem*> m_profileItems;
    QHash<QString, Tp::ConnectionManagerPtr> m_connectionManagers;
};


#endif // header guard


