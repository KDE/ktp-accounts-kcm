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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_PARAMETER_ITEM_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_PARAMETER_ITEM_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QValidator>

#include <TelepathyQt/ConnectionManager>
#include <TelepathyQt/Profile>


class ParameterItem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ParameterItem);

public:
    ParameterItem(const Tp::ProtocolParameter &parameter,
                  const Tp::Profile::Parameter &profileParameter,
                  const QVariant &originalValue,
                  QObject *parent = 0);
    virtual ~ParameterItem();

    QString name() const;
    QString localizedName() const;
    QVariant::Type type() const;
    QVariant value() const;
    bool isSecret() const;
    bool isRequired() const;
    bool isRequiredForRegistration() const;
    /** Returns true if the item is from a profile and should _not_ be changed*/
    bool isMandatory() const;
    const Tp::ProtocolParameter parameter() const;
    const Tp::Profile::Parameter profileParameter() const;
    QValidator::State validity() const;

    void setValue(const QVariant &value);
    void setValidity(QValidator::State validity);

private:
    Tp::ProtocolParameter m_parameter;
    Tp::Profile::Parameter m_profileParameter;
    const QVariant m_originalValue;
    QVariant m_currentValue;
    QString m_localizedName;
    QValidator::State m_validity;
};


#endif // header guard

