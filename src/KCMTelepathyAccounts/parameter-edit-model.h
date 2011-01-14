/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Thomas Richard
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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_PARAMETER_EDIT_MODEL_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_PARAMETER_EDIT_MODEL_H

#include "protocol-parameter-value.h"

#include <QtCore/QAbstractListModel>

class ParameterItem;

namespace Tp {
    class ProtocolParameter;
}

class KDE_EXPORT ParameterEditModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ParameterEditModel);

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        LocalizedNameRole,
        TypeRole,
        ValueRole,
        DefaultValueRole,
        SecretRole,
        RequiredRole,
        RequiredForRegistrationRole,
        ValidityRole,
        UserRole = Qt::UserRole + 42
    };

    explicit ParameterEditModel(QObject *parent = 0);
    virtual ~ParameterEditModel();

    virtual int rowCount(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex indexForParameter(const Tp::ProtocolParameter &parameter) const;
    Tp::ProtocolParameter parameter(const QString &parameterName) const;

    void addItem(const Tp::ProtocolParameter &parameter, const QVariant &originalValue);
    void addItems(const Tp::ProtocolParameterList &parameters, const QVariantMap &parameterValues = QVariantMap());
    QList<ProtocolParameterValue> parameterValues() const;
    Tp::ProtocolParameterList parameters() const;

    bool validateParameterValues();

private:
    QList<ParameterItem*> m_items;
};


#endif // header guard

