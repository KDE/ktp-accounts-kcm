/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#include <KDebug>


#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>
#include <KCMTelepathyAccounts/GenericAdvancedOptionsWidget>

#include "icq-main-options-widget.h"
#include "icq-server-settings-widget.h"

#include "haze-icq-account.h"



class HazeIcqAccountUi::Private
{
public:
    Private()
    {
        kDebug();
    }
};

HazeIcqAccountUi::HazeIcqAccountUi(QObject *parent)
 : AbstractAccountUi(parent),
   d(new Private)
{
    kDebug();

    // Register supported parameters
    registerSupportedParameter("account", QVariant::String);
    registerSupportedParameter("password", QVariant::String);

    registerSupportedParameter("port", QVariant::Int);
    registerSupportedParameter("server", QVariant::String);

    registerSupportedParameter("use-ssl", QVariant::Bool);
    registerSupportedParameter("always-use-rv-proxy", QVariant::Bool);
    registerSupportedParameter("allow-multiple-logins", QVariant::Bool);
    registerSupportedParameter("charset", QVariant::String);
}

HazeIcqAccountUi::~HazeIcqAccountUi()
{
    kDebug();

    delete d;
}

AbstractAccountParametersWidget *HazeIcqAccountUi::mainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    kDebug();

    return new IcqMainOptionsWidget(model, parent);
}

bool HazeIcqAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *HazeIcqAccountUi::advancedOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    kDebug();

    GenericAdvancedOptionsWidget *advancedOptionsWidget = new GenericAdvancedOptionsWidget(model, parent);
    AbstractAccountParametersWidget *icqServerSettingsWidget = new IcqServerSettingsWidget(model, parent);
    advancedOptionsWidget->addTab(icqServerSettingsWidget, "Server");
    return advancedOptionsWidget;
}


#include "haze-icq-account.moc"

