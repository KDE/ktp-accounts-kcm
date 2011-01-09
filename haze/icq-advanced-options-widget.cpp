/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "icq-advanced-options-widget.h"
#include "icq-server-settings-widget.h"

#include <KCMTelepathyAccounts/ProtocolParameterValue>

#include <KDebug>
#include <KTabWidget>
#include <KLocale>

#include <QHBoxLayout>


class IcqAdvancedOptionsWidget::Private
{
public:
    Private()
            : serverSettings(0)
    {
        kDebug();
    }
    IcqServerSettingsWidget *serverSettings;
};

IcqAdvancedOptionsWidget::IcqAdvancedOptionsWidget(Tp::ProtocolParameterList parameters,
                                             const QVariantMap &values,
                                             QWidget *parent)
 : AbstractAccountParametersWidget(parameters, values, parent),
   d(new Private)
{
    // add a layout to the main widget
    QHBoxLayout *layout = new QHBoxLayout(this);
    d->serverSettings = new IcqServerSettingsWidget(parameters, values, 0);
    layout->addWidget(d->serverSettings);
}

IcqAdvancedOptionsWidget::~IcqAdvancedOptionsWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> IcqAdvancedOptionsWidget::parameterValues() const
{
    kDebug();

    // the server parameter values
    QList<ProtocolParameterValue> parameters;
    parameters.append(d->serverSettings->parameterValues());

    return parameters;
}

bool IcqAdvancedOptionsWidget::validateParameterValues()
{
    kDebug();

    // validate one tab at a time so that the user is not flooded with dialogs
    if (!d->serverSettings->validateParameterValues())
        return false;

    return true;
}

#include "icq-advanced-options-widget.moc"

