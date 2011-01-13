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

#include "profile-item.h"

#include "profile-list-model.h"
#include "connection-manager-item.h"
#include "dictionary.h"

#include <TelepathyQt4/Profile>
#include <KDebug>

ProfileItem::ProfileItem(const Tp::ProfilePtr &profile, ProfileListModel *parent)
 : QObject(parent),
   m_profile(profile)
{
    kDebug() << "Creating new ProfileItem with and profile;" << profile.data()->name();

    m_localizedName = Dictionary::instance()->string(profile.data()->name());
    if(m_localizedName.isEmpty()) {
        m_localizedName = profile.data()->name();
    }
}

ProfileItem::~ProfileItem()
{
    kDebug();

    // TODO: Implement me...
}

QString ProfileItem::name() const
{
    return m_profile.data()->name();
}

QString ProfileItem::localizedName() const
{
    return m_localizedName;
}

QString ProfileItem::iconName() const
{
    return m_profile.data()->iconName();
}

QString ProfileItem::cmName() const
{
    return m_profile.data()->cmName();
}

QString ProfileItem::protocolName() const
{
    return m_profile.data()->protocolName();
}

#include "profile-item.moc"

