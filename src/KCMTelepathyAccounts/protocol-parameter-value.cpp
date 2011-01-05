/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson. <kde@davidedmundson.co.uk>
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

#include "protocol-parameter-value.h"

class ProtocolParameterValue::Private
{
public:
    Tp::ProtocolParameter parameter;
    QVariant value;
};


ProtocolParameterValue::ProtocolParameterValue(const Tp::ProtocolParameter &parameter, const QVariant value)
    :d (new Private)
{
    d->parameter = parameter;
    d->value = value;
    //make sure we're storing the correct type of QVariant type for this parameter
    d->value.convert(d->parameter.type());
}

ProtocolParameterValue::ProtocolParameterValue(const ProtocolParameterValue &other)
    :d (new Private(*other.d))
{

}

ProtocolParameterValue::~ProtocolParameterValue()
{
    delete d;
}

ProtocolParameterValue &ProtocolParameterValue::operator=(const ProtocolParameterValue &other)
{
    *d = *other.d;
    return *this;
}


const Tp::ProtocolParameter ProtocolParameterValue::parameter() const
{
    return d->parameter;
}

const QVariant ProtocolParameterValue::value() const
{
    return d->value;
}

const QString ProtocolParameterValue::name() const
{
    QString name;
    if(d->parameter.isValid()) {
        name = d->parameter.name();
    }
    return name;
}

bool ProtocolParameterValue::shouldSave() const
{
    if (! d->parameter.isValid()) {
        return false;
    }

    if (! d->value.isValid()) {
        return false;
    }

    if (d->value.isNull()) {
        return false;
    }

    // Don't any parameters where the default value is equal to the current value.
    if (d->parameter.defaultValue() == d->value) {
        return false;
    }

    // Don't save any strings where the default is empty, and the value is an empty string
    if (d->parameter.type() == QVariant::String) {
        if ((d->parameter.defaultValue().isNull()) && d->value.toString().isEmpty()) {
            return false;
        }
    }

    return true;
}
