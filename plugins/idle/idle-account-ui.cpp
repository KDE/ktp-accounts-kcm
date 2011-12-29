/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Lasse Liehu <lliehu@kolumbus.fi>
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

#include "idle-account-ui.h"
#include "main-options-widget.h"
#include "advanced-options-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

IdleAccountUi::IdleAccountUi(QObject *parent)
 : AbstractAccountUi(parent)
{
    // Register supported parameters
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("server"), QVariant::String);
    registerSupportedParameter(QLatin1String("port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("username"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);
    registerSupportedParameter(QLatin1String("fullname"), QVariant::String);
    registerSupportedParameter(QLatin1String("use-ssl"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("charset"), QVariant::String);
//     registerSupportedParameter(QLatin1String("quit-message"), QVariant::String);
}

IdleAccountUi::~IdleAccountUi()
{
}

AbstractAccountParametersWidget *IdleAccountUi::mainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    return new MainOptionsWidget(model, parent);
}

bool IdleAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *IdleAccountUi::advancedOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    return new AdvancedOptionsWidget(model, parent);
}

#include "idle-account-ui.moc"
