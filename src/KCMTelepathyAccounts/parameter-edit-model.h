/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#include "kcm_telepathy_accounts_export.h"

#include <QtCore/QAbstractListModel>

#include <TelepathyQt/Profile>
#include <TelepathyQt/ProtocolParameter>

#include <KMessageWidget>

class ParameterItem;

namespace Tp {
    class Profile;
    class Parameter;
}

class KCM_TELEPATHY_ACCOUNTS_EXPORT ParameterEditModel : public QAbstractListModel
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
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex indexForParameter(const Tp::ProtocolParameter &parameter) const;
    QModelIndex indexForParameter(const Tp::Profile::Parameter &parameter) const;
    Tp::ProtocolParameter parameter(const QString &parameterName) const;

    void addItem(const Tp::ProtocolParameter &parameter, const Tp::Profile::Parameter &profileParameter, const QVariant &originalValue);
    void addItems(const Tp::ProtocolParameterList &parameters, const Tp::Profile::ParameterList &profileParameters, const QVariantMap &parameterValues = QVariantMap());

    Tp::ProtocolParameterList parameters() const;

    /** Returns a map of the parameters values, keyed by parameter name in a format suitable for sending to Telepathy*/
    QVariantMap parametersSet() const;

    /** Returns a list of the names of parameters which are handled by the model, but should not be saved in the user's settings*/
    QStringList parametersUnset() const;


    bool validateParameterValues();

Q_SIGNALS:
    void feedbackMessage(const QString &text, const QString &comment, KMessageWidget::MessageType);

private:
    QList<ParameterItem*> m_items;

    /** Returns true if this parameter/value pair should be saved. False if it should be unset*/
    bool parameterNeedsSaving(const Tp::ProtocolParameter &parameter, const QVariant &value) const;
};


#endif // header guard

