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

#ifndef TELEPATHY_ACCOUNTS_KCM_PROFILE_LIST_MODEL_H
#define TELEPATHY_ACCOUNTS_KCM_PROFILE_LIST_MODEL_H

#include <QtCore/QAbstractListModel>

#include <TelepathyQt4/ConnectionManager>

class ProfileItem;

class ProfileListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ProfileListModel);

public:
    explicit ProfileListModel(QObject *parent = 0);
    virtual ~ProfileListModel();

    virtual int rowCount(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    ProfileItem *itemForIndex(const QModelIndex &index) const;

    void setProfileManager(Tp::ProfileManagerPtr profileManager);

private:
    bool hasNonFakeProfile(const Tp::ProfilePtr &profile, const QList<Tp::ProfilePtr> &profiles) const;

    QList<ProfileItem*> m_profileItems;
};


#endif // header guard


