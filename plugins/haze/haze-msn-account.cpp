/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Francesco Nwokeka <francesco.nwokeka@gmail.com>
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

#include "haze-msn-account.h"
#include "msn-main-options-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

HazeMsnAccountUi::HazeMsnAccountUi(QObject* parent)
 : AbstractAccountUi(parent)
{
    // Register supported parameters
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);
}

HazeMsnAccountUi::~HazeMsnAccountUi()
{
}

// AbstractAccountParametersWidget *HazeMsnAccountUi::advancedOptionsWidget(ParameterEditModel *model, QWidget *parent) const
// {
//     AbstractAccountParametersWidget *msnServerSettingsWidget = new MsnServerSettingsWidget(model, parent);
//     return msnServerSettingsWidget;
// }

bool HazeMsnAccountUi::hasAdvancedOptionsWidget() const
{
    return false;
}

AbstractAccountParametersWidget *HazeMsnAccountUi::mainOptionsWidget(ParameterEditModel *model, QWidget *parent) const
{
    return new MsnMainOptionsWidget(model, parent);
}

#include "haze-msn-account.moc"
