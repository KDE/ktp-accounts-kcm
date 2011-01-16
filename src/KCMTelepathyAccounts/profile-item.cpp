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

#include "profile-item.h"

#include "profile-list-model.h"
#include "dictionary.h"

#include <TelepathyQt4/Profile>
#include <KDebug>
#include <KIcon>

ProfileItem::ProfileItem(const Tp::ProfilePtr &profile, ProfileListModel *parent)
 : QObject(parent),
   m_profile(profile),
   m_icon(0)
{
    //FIXME: Dictionary should not be needed anymore when distros ship profiles
    m_localizedName = Dictionary::instance()->string(profile->name());
    if(m_localizedName.isEmpty()) {
        m_localizedName = profile->name();
    }

    generateIcon();
}

ProfileItem::~ProfileItem()
{
    kDebug();

    delete m_icon;
}

QString ProfileItem::serviceName() const
{
    return m_profile->serviceName();
}

QString ProfileItem::name() const
{
    return m_profile->name();
}

QString ProfileItem::localizedName() const
{
    return m_localizedName;
}

QString ProfileItem::iconName() const
{
    return m_profile->iconName();
}

QString ProfileItem::cmName() const
{
    return m_profile->cmName();
}

QString ProfileItem::protocolName() const
{
    return m_profile->protocolName();
}

Tp::ProfilePtr ProfileItem::profile() const
{
    return m_profile;
}

const KIcon& ProfileItem::icon() const
{
    Q_ASSERT(m_icon != 0);

    return *m_icon;
}

void ProfileItem::generateIcon()
{
    QString iconPath = profile()->iconName();

    delete m_icon;
    m_icon = new KIcon(iconPath);
}

#include "profile-item.moc"

