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

#include "abstract-account-ui.h"

#include <KDebug>

class AbstractAccountUi::Private
{
public:
    Private()
    {
        kDebug();
    }

    QMap<QString, QVariant::Type> supportedMandatoryParameters;
    QMap<QString, QVariant::Type> supportedOptionalParameters;
};

AbstractAccountUi::AbstractAccountUi(QObject *parent)
 : QObject(parent),
   d(new Private)
{
    kDebug();
}

AbstractAccountUi::~AbstractAccountUi()
{
    kDebug();

    delete d;
}

const QMap<QString, QVariant::Type> &AbstractAccountUi::supportedMandatoryParameters() const
{
    return d->supportedMandatoryParameters;
}

const QMap<QString, QVariant::Type> &AbstractAccountUi::supportedOptionalParameters() const
{
    return d->supportedOptionalParameters;
}

void AbstractAccountUi::registerSupportedMandatoryParameter(const QString &name, QVariant::Type type)
{
    // Check that the parameter is not already in the list
    foreach (QVariant::Type t, d->supportedMandatoryParameters.values(name)) {
        if (t == type) {
            kWarning() << "Parameter:" << name << "of type:" << type << "is already added.";
            return;
        }
    }

    // Add the parameter to the list.
    d->supportedMandatoryParameters.insertMulti(name, type);
}

void AbstractAccountUi::registerSupportedOptionalParameter(const QString &name, QVariant::Type type)
{
    // Check that the parameter is not already in the list
    foreach (QVariant::Type t, d->supportedOptionalParameters.values(name)) {
        if (t == type) {
            kWarning() << "Parameter:" << name << "of type:" << type << "is already added.";
            return;
        }
    }

    // Add the parameter to the list.
    d->supportedOptionalParameters.insertMulti(name, type);
}


#include "abstract-account-ui.moc"

