/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Lasath Fernando <kde@lasath.org>
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

#include "haze-yahoo-account.h"
#include "yahoo-server-settings-widget.h"

HazeYahooAccount::HazeYahooAccount(QObject* parent)
        : AbstractAccountUi(parent)
{
    //register that all options are supported
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);
    registerSupportedParameter(QLatin1String("port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("xfer-host"), QVariant::String);
    registerSupportedParameter(QLatin1String("xfer-port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("room-list-locale"), QVariant::String);
    registerSupportedParameter(QLatin1String("charset"), QVariant::String);
    registerSupportedParameter(QLatin1String("proxy-ssl"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("ignore-invites"), QVariant::Bool);
}

HazeYahooAccount::~HazeYahooAccount()
{
}

bool HazeYahooAccount::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget* HazeYahooAccount::mainOptionsWidget(
        ParameterEditModel* model,
        QWidget* parent) const
{
    return new YahooMainOptionsWidget(model,parent);
}

AbstractAccountParametersWidget* HazeYahooAccount::advancedOptionsWidget(
        ParameterEditModel* model,
        QWidget* parent) const
{
    AbstractAccountParametersWidget* yahooserversettings = new YahooServerSettingsWidget(model, parent);
    return yahooserversettings;
}

#include "haze-yahoo-account.moc"
