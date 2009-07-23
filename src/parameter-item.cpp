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

#include "parameter-item.h"

#include <KDebug>

ParameterItem::ParameterItem(Tp::ProtocolParameter *parameter,
                             const QVariant &originalValue,
                             QObject *parent)
 : QObject(parent),
   m_parameter(parameter),
   m_originalValue(originalValue)
{
    kDebug();

    // To begin with, the current value is the original value.
    m_currentValue = m_originalValue;
}

ParameterItem::~ParameterItem()
{
    kDebug();
}

QString ParameterItem::name() const
{
    return m_parameter->name();
}

