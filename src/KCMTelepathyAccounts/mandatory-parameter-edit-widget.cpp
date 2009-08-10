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

#include "mandatory-parameter-edit-widget.h"

#include <KDebug>
#include <KLocale>
#include <KMessageBox>

class MandatoryParameterEditWidget::Private
{
public:
    Private()
    {
        kDebug();
    }
};

MandatoryParameterEditWidget::MandatoryParameterEditWidget(Tp::ProtocolParameterList parameters,
                                                           const QVariantMap &values,
                                                           QWidget *parent)
 : ParameterEditWidget(parameters, values, parent),
   d(new Private)
{
    kDebug();
}

MandatoryParameterEditWidget::~MandatoryParameterEditWidget()
{
    kDebug();

    delete d;
}

bool MandatoryParameterEditWidget::validateParameterValues()
{
    kDebug();

    foreach (const QVariant &value, parameterValues().values()) {
        if (value.toString().isEmpty()) {
            KMessageBox::error(this, i18n("Please enter all mandatory parameters."));
            return false;
        }
    }

    return true;
}


#include "mandatory-parameter-edit-widget.moc"

