/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#include "parameter-edit-model.h"

#include "parameter-item.h"

#include <KLocalizedString>
#include <KMessageWidget>

#include <QDebug>
#include <QValidator>

#include <TelepathyQt/Profile>
#include <TelepathyQt/ConnectionManager>

ParameterEditModel::ParameterEditModel(QObject *parent)
 : QAbstractListModel(parent)
{
}

ParameterEditModel::~ParameterEditModel()
{
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
    if (!index.isValid()) {
        return QVariant();
    }

    // FIXME: This is a basic implementation just so I can see what's going
    // on while developing this code further. Needs expanding.
    QVariant data;

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case ParameterEditModel::ValueRole:
        data = QVariant(m_items.at(index.row())->value());
        if (!data.isValid()) {
            data = this->data(index, ParameterEditModel::DefaultValueRole);
        }
        break;
    case ParameterEditModel::DefaultValueRole:
        data = QVariant(m_items.at(index.row())->parameter().defaultValue());
        break;
    case ParameterEditModel::NameRole:
        data = QVariant(m_items.at(index.row())->name());
        break;
    case ParameterEditModel::LocalizedNameRole:
        data = QVariant(m_items.at(index.row())->localizedName());
        break;
    case ParameterEditModel::TypeRole:
        data = QVariant(m_items.at(index.row())->type());
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

Qt::ItemFlags ParameterEditModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = 0;
    if (!index.isValid()) {
        return flags;
    }

    flags |= Qt::ItemIsSelectable;

    if (! m_items.at(index.row())->isMandatory()) {
        flags |= Qt::ItemIsEnabled;
    }
    return flags;
}

bool ParameterEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qDebug() << "Invalid item row accessed.";
        return false;
    }

    if (role == ParameterEditModel::ValueRole || role == Qt::EditRole) {
        //never edit items fixed in the profile.
        if (m_items.at(index.row())->isMandatory()) {
            return false;
        }

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


QModelIndex ParameterEditModel::indexForParameter(const Tp::ProtocolParameter &parameter) const
{
    for (int i=0; i<m_items.size(); ++i) {
        if (m_items.at(i)->parameter() == parameter) {
            return createIndex(i,0);
        }
    }
    return QModelIndex();
}

QModelIndex ParameterEditModel::indexForParameter(const Tp::Profile::Parameter &parameter) const
{
    for (int i=0; i<m_items.size(); ++i) {
        ParameterItem *item = m_items.at(i);
        if (item->parameter().name() == parameter.name() && item->parameter().type() == parameter.type()) {
            return createIndex(i,0);
        }
    }
    return QModelIndex();
}

Tp::ProtocolParameter ParameterEditModel::parameter(const QString &parameterName) const
{
    Q_FOREACH (ParameterItem* item, m_items) {
        if (item->parameter().name() == parameterName) {
            return item->parameter();
        }
    }

    return Tp::ProtocolParameter();
}


void ParameterEditModel::addItem(const Tp::ProtocolParameter &parameter, const Tp::Profile::Parameter &profileParameter, const QVariant &originalValue)
{
    // Check we are not creating duplicate items.
    Q_FOREACH (const ParameterItem *item, m_items) {
        if (item->parameter() == parameter) {
            return;
        }
    }

    // Create a new ParameterItem and add it to the list.
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(new ParameterItem(parameter, profileParameter, originalValue, this));
    endInsertRows();
}

void ParameterEditModel::addItems(const Tp::ProtocolParameterList &parameters, const Tp::Profile::ParameterList &profileParameters, const QVariantMap &parameterValues)
{
    Q_FOREACH (const Tp::ProtocolParameter &parameter, parameters) {

        Tp::Profile::Parameter relevantProfileParameter;

        //try and find the correct profile parameter, if it can't be found leave it as empty.
        Q_FOREACH (const Tp::Profile::Parameter &profileParameter, profileParameters) {
            if (profileParameter.name() == parameter.name()) {
                relevantProfileParameter = profileParameter;
                break;
            }
        }

        addItem(parameter, relevantProfileParameter, parameterValues[parameter.name()]);
    }
}

Tp::ProtocolParameterList ParameterEditModel::parameters() const
{
    Tp::ProtocolParameterList parameters;

    Q_FOREACH (ParameterItem *item, m_items) {
        parameters.append(item->parameter());
    }

    return parameters;
}

QVariantMap ParameterEditModel::parametersSet() const
{
    QVariantMap setParameters;
    Q_FOREACH (ParameterItem *item, m_items) {
        Tp::ProtocolParameter parameter = item->parameter();
        QVariant value = item->value();

        //return the parameter value in the type Tp is expecting
        value.convert(parameter.type());

        if (parameterNeedsSaving(parameter, value)) {
            setParameters.insert(parameter.name(), value);
        }
    }
    return setParameters;
}

QStringList ParameterEditModel::parametersUnset() const
{
    QStringList unsetParameters;
    Q_FOREACH (ParameterItem *item, m_items) {
        Tp::ProtocolParameter parameter = item->parameter();
        QVariant value = item->value();

        if (! parameterNeedsSaving(parameter, value)) {
            unsetParameters.append(parameter.name());
        }
    }

    return unsetParameters;
}

bool ParameterEditModel::parameterNeedsSaving(const Tp::ProtocolParameter &parameter, const QVariant &value) const
{
    if (! parameter.isValid()) {
        return false;
    }

    if (! value.isValid()) {
        return false;
    }

    if (value.isNull()) {
        return false;
    }

    // Don't any parameters where the default value is equal to the current value.
    if (parameter.defaultValue() == value) {
        return false;
    }

    // Don't save any strings where the default is empty, and the value is an empty string
    if (parameter.type() == QVariant::String) {
        if ((parameter.defaultValue().isNull()) && value.toString().isEmpty()) {
            return false;
        }
    }

    return true;
}


bool ParameterEditModel::validateParameterValues()
{
    Q_FOREACH (ParameterItem *item, m_items) {
        if (item->validity() != QValidator::Acceptable) {
           qWarning() << "Parameter" << item->name() << "is not valid:" << item->value();
           Q_EMIT feedbackMessage(i18n("Parameter \"<b>%1</b>\" is not valid.",
                                item->localizedName()),
                                QString(),
                                KMessageWidget::Error);
           return false;
       }
    }

    return true;
}


#include "parameter-edit-model.moc"

