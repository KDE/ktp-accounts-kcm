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

#include "haze-icq-account.h"

#include "icq-main-options-widget.h"
#include "icq-server-settings-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

HazeIcqAccountUi::HazeIcqAccountUi(QObject *parent)
 : AbstractAccountUi(parent)
{
    // Register supported parameters
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);

    registerSupportedParameter(QLatin1String("port"), QVariant::Int);
    registerSupportedParameter(QLatin1String("server"), QVariant::String);

    registerSupportedParameter(QLatin1String("use-ssl"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("always-use-rv-proxy"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("allow-multiple-logins"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("charset"), QVariant::String);
}

HazeIcqAccountUi::~HazeIcqAccountUi()
{
}

AbstractAccountParametersWidget *HazeIcqAccountUi::mainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
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
    AbstractAccountParametersWidget *icqServerSettingsWidget = new IcqServerSettingsWidget(model, parent);
    return icqServerSettingsWidget;
}


#include "haze-icq-account.moc"
