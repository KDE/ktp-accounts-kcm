/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
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

#include "haze-skype-account-ui.h"

#include "skype-main-options-widget.h"
#include "skype-advanced-settings-widget.h"

#include <KDebug>

HazeSkypeAccountUi::HazeSkypeAccountUi(QObject *parent)
 : AbstractAccountUi(parent)
{
    kDebug();

    // Register supported parameters
    // Main Options
    registerSupportedParameter("account", QVariant::String);

    // Advanced Options
    registerSupportedParameter("skypeout-online", QVariant::Bool);
    registerSupportedParameter("skype-sync", QVariant::Bool);
    registerSupportedParameter("check-for-updates", QVariant::Bool);
    registerSupportedParameter("reject-all-auths", QVariant::Bool);
    registerSupportedParameter("skype-autostart", QVariant::Bool);
}

HazeSkypeAccountUi::~HazeSkypeAccountUi()
{
    kDebug();
}

AbstractAccountParametersWidget *HazeSkypeAccountUi::mainOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    kDebug();

    return new SkypeMainOptionsWidget(model, parent);
}

bool HazeSkypeAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *HazeSkypeAccountUi::advancedOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    kDebug();

    AbstractAccountParametersWidget *skypeAdvancedSettingsWidget = new SkypeAdvancedSettingsWidget(model, parent);
    return skypeAdvancedSettingsWidget;
}


#include "haze-skype-account-ui.moc"

