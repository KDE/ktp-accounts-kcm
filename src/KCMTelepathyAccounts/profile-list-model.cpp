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

#include "profile-list-model.h"

#include "profile-item.h"

#include <TelepathyQt/Feature>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ProfileManager>
#include <TelepathyQt/PendingStringList>
#include <TelepathyQt/PendingComposite>

#include <QDebug>
#include <QIcon>

ProfileListModel::ProfileListModel(QObject *parent)
 : QAbstractListModel(parent)
{
    m_profileItems.clear();
    m_profileManager = Tp::ProfileManager::create(QDBusConnection::sessionBus());
    // FIXME: Until all distros ship correct profile files, we should fake them
    connect(m_profileManager->becomeReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles),
            SIGNAL(finished(Tp::PendingOperation*)),
            this,
            SLOT(onProfileManagerReady(Tp::PendingOperation*)));
}

ProfileListModel::~ProfileListModel()
{
}

void ProfileListModel::onProfileManagerReady(Tp::PendingOperation *op)
{
    // Check the pending operation completed successfully.
    if (op->isError()) {
        qDebug() << "becomeReady() failed:" << op->errorName() << op->errorMessage();
        return;
    }
    Tp::PendingStringList* pendingNames = Tp::ConnectionManager::listNames();
    connect(pendingNames, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onConnectionManagerNamesFetched(Tp::PendingOperation*)));
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

    switch (role) {
    case Qt::DisplayRole:
        data = QVariant(m_profileItems.at(index.row())->localizedName());
        break;
    case Qt::DecorationRole:
        data = QVariant(m_profileItems.at(index.row())->icon());
        break;
    case ProfileListModel::ProfileProtocolNameRole:
        data = QVariant(m_profileItems.at(index.row())->protocolName());
        break;
    case ProfileListModel::ProfileCmNameRole:
        data = QVariant(m_profileItems.at(index.row())->cmName());
        break;
    default:
        break;
    }

    return data;
}

ProfileItem *ProfileListModel::itemForIndex(const QModelIndex &index) const
{
    return m_profileItems.at(index.row());
}

ProfileItem *ProfileListModel::itemForService(const QString &serviceName) const
{
    Q_FOREACH (ProfileItem *profileItem, m_profileItems) {
        if (profileItem->serviceName() == serviceName) {
            return profileItem;
        }
    }

    return nullptr;
}

bool ProfileListModel::hasNonFakeProfile(const Tp::ProfilePtr& profile, const QList<Tp::ProfilePtr> &profiles) const
{
    //loop through all profiles, and look for a non autogenerated profile which matches this name.
    Q_FOREACH (const Tp::ProfilePtr &otherProfile, profiles) {
        if (profile->protocolName() == otherProfile->protocolName() && !otherProfile->isFake()) {
            // check if this profile is for a special service or for this protocol in general
            if (otherProfile->serviceName() == otherProfile->cmName().append(QLatin1Char('-')).append(otherProfile->protocolName())
            || otherProfile->serviceName() == otherProfile->protocolName()) {

                //check we have a valid CM for the non-fake profile

                if (hasValidCM(otherProfile)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool ProfileListModel::hasValidCM(const Tp::ProfilePtr &profile) const
{
    //check we have a valid CM for the non-fake profile
    if (!m_connectionManagers.contains(profile->cmName())) {
        return false;
    }

    //if cm exists but doesn't support protocol in profile.
    if (!m_connectionManagers[profile->cmName()]->hasProtocol(profile->protocolName())) {
        return false;
    }


    return true;
}

void ProfileListModel::populateList()
{
    Q_FOREACH (ProfileItem *item, m_profileItems) {
        delete item;
        item = nullptr;
    }

    m_profileItems.clear();

    QList<Tp::ProfilePtr> profiles = m_profileManager->profiles();

    QList<ProfileItem*> insertItems;
    Q_FOREACH (const Tp::ProfilePtr &profile, profiles) {
        if (profile->isFake()) {
            if (hasNonFakeProfile(profile, profiles)) {
                continue;
            }
        }


        //don't include profiles which we don't have a CM for
        if (!hasValidCM(profile)) {
            continue;
        }

        //Hide all IRC accounts
        //this is a deliberate decision from Akademy meeting 2012
        //"no, we don't support IRC", it's a different usage and in order to have a semi-decent IRC experience
        //we need to add a lot more that we simply don't have resources to do.
        //It's a better user experience to learn how to use a different app than to try using this.

        //Remove this "continue" to re-enable IRC support, for personal reasons or hacking
        //this topic can be discussed again as of July 2013

        if(profile->protocolName() == QLatin1String("irc")) {
                continue;
        }

        insertItems.append(new ProfileItem(profile, this));
    }

    if (insertItems.size() > 0) {
      beginInsertRows(QModelIndex(), 0, insertItems.size()-1);
      m_profileItems.append(insertItems);
      endInsertRows();
    } else {
      return;
    }
}

void ProfileListModel::onConnectionManagerNamesFetched(Tp::PendingOperation *operation)
{
    Tp::PendingStringList* connectionManagerNamesOperation = qobject_cast<Tp::PendingStringList*>(operation);

    Q_ASSERT(connectionManagerNamesOperation);

    QList<Tp::PendingOperation*> pendingOps;

    Q_FOREACH(const QString &cmName, connectionManagerNamesOperation->result()) {
        Tp::ConnectionManagerPtr cm = Tp::ConnectionManager::create(QDBusConnection::sessionBus(), cmName);
        pendingOps.append(cm->becomeReady());
        m_connectionManagers[cmName] = cm;
    }

    connect(new Tp::PendingComposite(pendingOps, m_profileManager), SIGNAL(finished(Tp::PendingOperation*)), SLOT(onConnectionManagersLoaded(Tp::PendingOperation*)));
}

void ProfileListModel::onConnectionManagersLoaded(Tp::PendingOperation *op)
{
    Q_UNUSED(op)
    populateList();
}
