/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#include "dictionary.h"

ParameterItem::ParameterItem(const Tp::ProtocolParameter &parameter,
                             const Tp::Profile::Parameter &profileParameter,
                             const QVariant &originalValue,
                             QObject *parent)
 : QObject(parent),
   m_parameter(parameter),
   m_profileParameter(profileParameter),
   m_originalValue(originalValue)
{
    // To begin with, the current value is the original value.
    m_currentValue = m_originalValue;

    //if no default value is set, use the value from the profile (if it exists)
    if (m_currentValue.isNull()) {
        m_currentValue = profileParameter.value();
    }

    // Set the localized name with the value from the dictionary if possible.
    m_localizedName = Dictionary::instance()->string(parameter.name());

    if (m_localizedName.isEmpty()) {
        m_localizedName = parameter.name();
    }

    // Assume the default/un-altered value is valid.
    m_validity = QValidator::Acceptable;
}

ParameterItem::~ParameterItem()
{
}

QString ParameterItem::name() const
{
    return m_parameter.name();
}

QString ParameterItem::localizedName() const
{
    return m_localizedName;
}

QVariant::Type ParameterItem::type() const
{
    return m_parameter.type();
}

QVariant ParameterItem::value() const
{
    return m_currentValue;
}

bool ParameterItem::isSecret() const
{
    return m_parameter.isSecret();
}

bool ParameterItem::isRequired() const
{
    return m_parameter.isRequired();
}

bool ParameterItem::isRequiredForRegistration() const
{
    return m_parameter.isRequiredForRegistration();
}

bool ParameterItem::isMandatory() const
{
    return m_profileParameter.isMandatory();
}

const Tp::ProtocolParameter ParameterItem::parameter() const
{
    return m_parameter;
}

const Tp::Profile::Parameter ParameterItem::profileParameter() const
{
    return m_profileParameter;
}

QValidator::State ParameterItem::validity() const
{
    return m_validity;
}

void ParameterItem::setValidity(QValidator::State validity)
{
    m_validity = validity;
}

void ParameterItem::setValue(const QVariant &value)
{
    m_currentValue = value;
}


#include "parameter-item.moc"

