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

#include "profile-list-model.h"

#include "profile-item.h"

#include <TelepathyQt4/ProfileManager>
#include <TelepathyQt4/PendingStringList>

#include <KIcon>
#include <KDebug>

ProfileListModel::ProfileListModel(QObject *parent)
 : QAbstractListModel(parent)
{
    kDebug();

    m_profileItems.clear();
}

ProfileListModel::~ProfileListModel()
{
    kDebug();
}

int ProfileListModel::rowCount(const QModelIndex &index) const
{
    // If the index is the root item, then return the row count.
    if (index == QModelIndex()) {
       return m_profileItems.size();
    }

    // Otherwise, return 0 (as this is a list model, so all items
    // are children of the root item).
    return 0;
}

QVariant ProfileListModel::data(const QModelIndex &index, int role) const
{
    // FIXME: This is a basic implementation just so I can see what's going
    // on while developing this code further. Needs expanding.
    QVariant data;

    switch(role)
    {
    case Qt::DisplayRole:
        data = QVariant(m_profileItems.at(index.row())->localizedName());
        break;
    case Qt::DecorationRole:
        data = QVariant(m_profileItems.at(index.row())->icon());
        break;
    default:
        break;
    }

    return data;
}

void ProfileListModel::setProfileManager(Tp::ProfileManagerPtr profileManager)
{
    m_profileManager = profileManager;
    Tp::PendingStringList* pendingNames = Tp::ConnectionManager::listNames();
    connect(pendingNames, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onConnectionManagerNamesFetched(Tp::PendingOperation*)));
}

ProfileItem *ProfileListModel::itemForIndex(const QModelIndex &index) const
{
    return m_profileItems.at(index.row());
}

bool ProfileListModel::hasNonFakeProfile(const Tp::ProfilePtr& profile, const QList<Tp::ProfilePtr> &profiles) const
{
    foreach(const Tp::ProfilePtr &otherProfile, profiles) {
        if(profile->protocolName() == otherProfile->protocolName() && !otherProfile->isFake())
        {
            // check if this profile is for a special service or for this protocol in general
            if(otherProfile->serviceName() == otherProfile->cmName().append("-").append(otherProfile->protocolName())
            || otherProfile->serviceName() == otherProfile->protocolName()) {
                return true;
            }
        }
    }

    return false;
}

void ProfileListModel::populateList()
{
    foreach (ProfileItem *item, m_profileItems) {
        delete item;
        item = 0;
    }

    m_profileItems.clear();

    QList<Tp::ProfilePtr> profiles = m_profileManager->profiles();

    QList<ProfileItem*> insertItems;
    foreach(const Tp::ProfilePtr &profile, profiles) {
        if(profile->isFake()) {
            if(hasNonFakeProfile(profile, profiles)) {
                continue;
            }
        }

        //don't include profiles which we don't have a CM for
        if (! m_connectionManagerNames.contains(profile->cmName())) {
            continue;
        }

        insertItems.append(new ProfileItem(profile, this));
    }

    if( insertItems.size() > 0 )
    {
      beginInsertRows(QModelIndex(), 0, insertItems.size()-1);
      m_profileItems.append(insertItems);
      endInsertRows();
    }
    else {
      return;
    }
}

void ProfileListModel::onConnectionManagerNamesFetched(Tp::PendingOperation *operation)
{
    Tp::PendingStringList* connectionManagerNamesOperation = qobject_cast<Tp::PendingStringList*>(operation);

    Q_ASSERT(connectionManagerNamesOperation);
    m_connectionManagerNames = connectionManagerNamesOperation->result();

    populateList();
}


#include "profile-list-model.moc"

