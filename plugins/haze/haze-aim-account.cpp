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

#include "haze-aim-account.h"

#include "aim-main-options-widget.h"
#include "aim-server-settings-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

HazeAimAccount::HazeAimAccount(QObject *parent)
 : AbstractAccountUi(parent)
{
    // Register supported parameters
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);
    registerSupportedParameter(QLatin1String("server"), QVariant::String);
    registerSupportedParameter(QLatin1String("port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("encryption"), QVariant::String);
    registerSupportedParameter(QLatin1String("always-use-rv-proxy"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("use-clientlogin"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("allow-multiple-logins"), QVariant::Bool);
}

HazeAimAccount::~HazeAimAccount()
{
}

AbstractAccountParametersWidget *HazeAimAccount::mainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    return new AimMainOptionsWidget(model, parent);
}

bool HazeAimAccount::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *HazeAimAccount::advancedOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    return new AimServerSettingsWidget(model, parent);
}
