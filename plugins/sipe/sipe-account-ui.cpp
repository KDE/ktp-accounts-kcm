/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2013 Maksim Melnikau <maxposedon@gmail.com>
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

#include "sipe-account-ui.h"

#include "sipe-main-options-widget.h"
#include "sipe-advanced-options-widget.h"

SipeAccountUi::SipeAccountUi(QObject *parent)
 : AbstractAccountUi(parent)
{
    // Main Options
    registerSupportedParameter(QLatin1String("account"),  QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);

    // Advanced Options
    registerSupportedParameter(QLatin1String("login"), QVariant::String);
    registerSupportedParameter(QLatin1String("server"), QVariant::String);
    registerSupportedParameter(QLatin1String("port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("transport"), QVariant::String);
    registerSupportedParameter(QLatin1String("useragent"), QVariant::String);
    registerSupportedParameter(QLatin1String("authentication"), QVariant::String);
    registerSupportedParameter(QLatin1String("single-sign-on"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("don't-publish-calendar"), QVariant::Bool);
}

SipeAccountUi::~SipeAccountUi()
{
}

AbstractAccountParametersWidget* SipeAccountUi::mainOptionsWidget(ParameterEditModel *model, QWidget *parent) const
{
    AbstractAccountParametersWidget *sipeMainOptionsWidget = new SipeMainOptionsWidget(model, parent);
    return sipeMainOptionsWidget;
}

bool SipeAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *SipeAccountUi::advancedOptionsWidget(ParameterEditModel *model, QWidget *parent) const
{
    AbstractAccountParametersWidget *sipeAdvancedOptionsWidget = new SipeAdvancedOptionsWidget(model, parent);
    return sipeAdvancedOptionsWidget;
}

#include "sipe-account-ui.moc"
