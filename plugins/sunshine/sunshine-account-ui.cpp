/*
 * This file is part of telepathy-accounts-kcm-plugins
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

#include "sunshine-account-ui.h"
#include "sunshine-main-options-widget.h"
#include "sunshine-advanced-options-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

SunshineAccountUi::SunshineAccountUi(QObject *parent)
    : AbstractAccountUi(parent)
{
    // Register supported parameters
    // Main Options
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);

    // Advanced Options
    registerSupportedParameter(QLatin1String("use-specified-server"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("server"), QVariant::String);
    registerSupportedParameter(QLatin1String("port"), QVariant::UInt);
    registerSupportedParameter(QLatin1String("use-ssl"), QVariant::Bool);
    registerSupportedParameter(QLatin1String("export-contacts"), QVariant::Bool);
    // People using telepathy-sunshine from master (past 0.1.8 versions) will need this line insead
    // of export-contacts. See the commit for further details:
    // http://git.collabora.co.uk/?p=telepathy-sunshine.git;a=commit;h=83963893c34c6069f7202cf7d55add2d334581b5
    // registerSupportedParameter(QLatin1String("synchronize-contacts-with-server"), QVariant::Bool);
}

SunshineAccountUi::~SunshineAccountUi()
{
}

AbstractAccountParametersWidget *SunshineAccountUi::mainOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    return new SunshineMainOptionsWidget(model, parent);
}

bool SunshineAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *SunshineAccountUi::advancedOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    AbstractAccountParametersWidget* aowidget = new SunshineAdvancedOptionsWidget(model, parent);

    return aowidget;
}

#include "sunshine-account-ui.moc"
