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

#include "advanced-options-widget.h"
#include "proxy-settings-widget.h"
#include "server-settings-widget.h"

#include <KDebug>
#include <KTabWidget>
#include <KLocale>

#include <QHBoxLayout>

class AdvancedOptionsWidget::Private
{
public:
    Private()
            : tabWidget(0),
              proxySettings(0),
              serverSettings(0)
    {
        kDebug();
    }
    KTabWidget *tabWidget;
    ProxySettingsWidget *proxySettings;
    ServerSettingsWidget *serverSettings;
};

AdvancedOptionsWidget::AdvancedOptionsWidget(Tp::ProtocolParameterList parameters,
                                             const QVariantMap &values,
                                             QWidget *parent)
 : AbstractAccountParametersWidget(parameters, values, parent),
   d(new Private)
{
    // add a layout to the main widget
    QHBoxLayout *layout = new QHBoxLayout(this);
    d->tabWidget = new KTabWidget(this);
    layout->addWidget(d->tabWidget);

    // create the pages
    d->serverSettings = new ServerSettingsWidget(parameters, values, d->tabWidget);
    d->tabWidget->addTab(d->serverSettings, i18n("Connection Settings"));
    d->proxySettings = new ProxySettingsWidget(parameters, values, d->tabWidget);
    d->tabWidget->addTab(d->proxySettings, i18n("Proxy Settings"));
}

AdvancedOptionsWidget::~AdvancedOptionsWidget()
{
    kDebug();

    delete d;
}

QVariantMap AdvancedOptionsWidget::parameterValues() const
{
    kDebug();

    // the server parameter values
    QVariantMap parameters;
    parameters = d->serverSettings->parameterValues();
    parameters.unite(d->proxySettings->parameterValues());

    return parameters;
}

bool AdvancedOptionsWidget::validateParameterValues()
{
    kDebug();

    // validate one tab at a time so that the user is not flooded with dialogs
    if (!d->serverSettings->validateParameterValues())
        return false;

    if (!d->proxySettings->validateParameterValues())
        return false;

    return true;
}

#include "advanced-options-widget.moc"

