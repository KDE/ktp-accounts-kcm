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

#include "salut-account-ui.h"
#include "salut-main-options-widget.h"
#include "salut-advanced-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>
#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

SalutAccountUi::SalutAccountUi(QObject *parent)
    : AbstractAccountUi(parent)
{
    // Register supported parameters
    // Main Options
    registerSupportedParameter("first-name", QVariant::String);
    registerSupportedParameter("last-name",  QVariant::String);
    registerSupportedParameter("nickname",   QVariant::String);

    // Advanced Options
    registerSupportedParameter("published-name", QVariant::String);
    registerSupportedParameter("email",          QVariant::String);
    registerSupportedParameter("jid",            QVariant::String);
}

SalutAccountUi::~SalutAccountUi()
{
}

AbstractAccountParametersWidget *SalutAccountUi::mainOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    return new SalutMainOptionsWidget(model, parent);
}

bool SalutAccountUi::hasAdvancedOptionsWidget() const
{
    return true;
}

AbstractAccountParametersWidget *SalutAccountUi::advancedOptionsWidget(
    ParameterEditModel *model,
    QWidget *parent) const
{
    AbstractAccountParametersWidget* aowidget = new SalutAdvancedOptionsWidget(model, parent);

    return aowidget;
}

#include "salut-account-ui.moc"
