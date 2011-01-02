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

#include <KApplication>
#include <KDebug>
#include <KLocale>
#include <KMessageBox>

#include <QtGui/QValidator>

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
    case ParameterEditModel::NameRole:
        data = QVariant(m_items.at(index.row())->name());
        break;
    case ParameterEditModel::LocalizedNameRole:
        data = QVariant(m_items.at(index.row())->localizedName());
        break;
    case ParameterEditModel::TypeRole:
        data = QVariant(m_items.at(index.row())->type());
        break;
    case ParameterEditModel::ValueRole:
        data = QVariant(m_items.at(index.row())->value());
        break;
    case ParameterEditModel::SecretRole:
        data = QVariant(m_items.at(index.row())->isSecret());
        break;
    case ParameterEditModel::RequiredRole:
        data = QVariant(m_items.at(index.row())->isRequired());
        break;
    case ParameterEditModel::RequiredForRegistrationRole:
        data = QVariant(m_items.at(index.row())->isRequiredForRegistration());
        break;
    case ParameterEditModel::ValidityRole:
        data = QVariant(m_items.at(index.row())->validity());
        break;
    default:
        break;
    }

    return data;
}

bool ParameterEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() == -1) {
        kDebug() << "Invalid item row accessed.";
        return false;
    }

    if (index.row() >= m_items.size()) {
        kWarning() << "Out of range row accessed." << index.row();
        return false;
    }

    if (role == ParameterEditModel::ValueRole) {

        m_items.at(index.row())->setValue(value);
        Q_EMIT dataChanged(index, index);
        return true;

    } else if (ParameterEditModel::ValidityRole) {

        if (value.toInt() == QValidator::Acceptable) {
            m_items.at(index.row())->setValidity(QValidator::Acceptable);
        } else if (value.toInt() == QValidator::Intermediate) {
            m_items.at(index.row())->setValidity(QValidator::Intermediate);
        } else {
            m_items.at(index.row())->setValidity(QValidator::Invalid);
        }

        Q_EMIT dataChanged(index, index);
        return true;

    } else {
        return false;
    }
}

void ParameterEditModel::addItem(Tp::ProtocolParameter parameter, const QVariant &originalValue)
{
    kDebug();
    // FIXME: Check we are not creating duplicate items.

    // Create a new ParameterItem and add it to the list.
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(new ParameterItem(parameter, originalValue, this));
    endInsertRows();
}

QVariantMap ParameterEditModel::parameterValues() const
{
    QVariantMap values;

    foreach (ParameterItem *item, m_items) {
        QVariant value = item->value();
        value.convert(item->type());
        values.insert(item->parameter().name(), value);
    }

    return values;
}

bool ParameterEditModel::validateParameterValues()
{
    foreach (ParameterItem *item, m_items) {
        if (item->validity() != QValidator::Acceptable) {
           // Display a more helpful error here.
           KMessageBox::error(QApplication::activeWindow (),
                              i18n("\"<b>%1</b>\" is not an acceptable value for <b>%2</b>")
                              .arg(item->value().toString())
                              .arg(item->localizedName()));
           return false;
       }
    }

    return true;
}


#include "parameter-edit-model.moc"

