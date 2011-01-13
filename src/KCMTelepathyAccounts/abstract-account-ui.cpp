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

#include "abstract-account-ui.h"

#include <KDebug>

class AbstractAccountUi::Private
{
public:
    Private()
    {
        kDebug();
    }
    QMap<QString, QVariant::Type> supportedParameters;
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

bool AbstractAccountUi::hasAdvancedOptionsWidget() const
{
    return false;
}

AbstractAccountParametersWidget* AbstractAccountUi::advancedOptionsWidget(ParameterEditModel *model,
                                                                          QWidget *parent) const
{
    return 0;
}

const QMap<QString, QVariant::Type> &AbstractAccountUi::supportedParameters() const
{
    return d->supportedParameters;
}

void AbstractAccountUi::registerSupportedParameter(const QString &name, QVariant::Type type)
{
    // Check that the parameter is not already in the list
    foreach (QVariant::Type t, d->supportedParameters.values(name)) {
        if (t == type) {
            kWarning() << "Parameter:" << name << "of type:" << type << "is already added.";
            return;
        }
    }

    // Add the parameter to the list.
    d->supportedParameters.insertMulti(name, type);
}



#include "abstract-account-ui.moc"

