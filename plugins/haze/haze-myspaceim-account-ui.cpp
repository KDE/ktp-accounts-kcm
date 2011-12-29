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

#include "haze-myspaceim-account-ui.h"

#include "myspaceim-main-options-widget.h"
#include "myspaceim-advanced-settings-widget.h"

HazeMySpaceIMAccountUi::HazeMySpaceIMAccountUi(QObject *parent)
 : AbstractAccountUi(parent)
{
    // Register supported parameters
    // Main Options
    registerSupportedParameter(QLatin1String("account"),  QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);

    // Advanced Options
    registerSupportedParameter(QLatin1String("server"), QVariant::String);
    registerSupportedParameter(QLatin1String("port"),   QVariant::UInt);
}

HazeMySpaceIMAccountUi::~HazeMySpaceIMAccountUi()
{
}

AbstractAccountParametersWidget *HazeMySpaceIMAccountUi::mainOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    return new MySpaceIMMainOptionsWidget(model, parent);
}

bool HazeMySpaceIMAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *HazeMySpaceIMAccountUi::advancedOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    AbstractAccountParametersWidget *myspaceimAdvancedSettingsWidget = new MySpaceIMAdvancedSettingsWidget(model, parent);
    return myspaceimAdvancedSettingsWidget;
}

#include "haze-myspaceim-account-ui.moc"
