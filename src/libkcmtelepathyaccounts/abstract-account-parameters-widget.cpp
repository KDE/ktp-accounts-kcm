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

#include "abstract-account-parameters-widget.h"

#include <KDebug>

class AbstractAccountParametersWidget::Private
{
public:
    Private()
    {
        kDebug();
    }

    Tp::ProtocolParameterList parameters;
};

AbstractAccountParametersWidget::AbstractAccountParametersWidget(Tp::ProtocolParameterList parameters,
                                                                 QWidget *parent)
        : QWidget(parent),
          d(new Private)
{
    kDebug();

    d->parameters = parameters;
}

AbstractAccountParametersWidget::~AbstractAccountParametersWidget()
{
    kDebug();

    delete d;
}

Tp::ProtocolParameterList AbstractAccountParametersWidget::parameters() const
{
    return d->parameters;
}


#include "abstract-account-parameters-widget.moc"

