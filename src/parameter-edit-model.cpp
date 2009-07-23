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

#include "parameter-edit-model.h"

#include "parameter-item.h"

#include <KDebug>

#include <TelepathyQt4/ConnectionManager>

ParameterEditModel::ParameterEditModel(QObject *parent)
 : QAbstractListModel(parent)
{
    kDebug();

    // TODO: Implement me!
}

ParameterEditModel::~ParameterEditModel()
{
    kDebug();

    // TODO: Implement me!
}

int ParameterEditModel::rowCount(const QModelIndex &index) const
{
    // If the index is the root item, then return the row count.
    if (index == QModelIndex()) {
       return m_items.size();
    }

    // Otherwise, return 0 (as this is a list model, so all items
    // are children of the root item).
    return 0;
}

QVariant ParameterEditModel::data(const QModelIndex &index, int role) const
{
    // FIXME: This is a basic implementation just so I can see what's going
    // on while developing this code further. Needs expanding.
    QVariant data;

    switch(role)
    {
    case Qt::DisplayRole:
        data = QVariant(m_items.at(index.row())->name());
        break;
    default:
        break;
    }

    return data;
}

void ParameterEditModel::addItem(Tp::ProtocolParameter *parameter, const QVariant &originalValue)
{
    kDebug();
    // FIXME: Check we are not creating duplicate items.

    // Create a new ParameterItem and add it to the list.
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(new ParameterItem(parameter, originalValue, this));
    endInsertRows();
}


#include "parameter-edit-model.moc"

