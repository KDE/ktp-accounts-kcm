/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "main-options-widget-msn.h"

#include <KCMTelepathyAccounts/ParameterEditModel>
#include <KLocalizedString>

MainOptionsWidgetMSN::MainOptionsWidgetMSN(ParameterEditModel *model,
                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // MSN uses OAUTH2 authentication and the only parameters that require
    // to be configured are mandatory and set using the profile
    // Therefore this UI is just empty

    // Set up the UI.
    m_ui = new Ui::MainOptionsWidgetMSN;
    m_ui->setupUi(this);
}

MainOptionsWidgetMSN::~MainOptionsWidgetMSN()
{
    delete m_ui;
}

void MainOptionsWidgetMSN::submit()
{
    AbstractAccountParametersWidget::submit();
}

bool MainOptionsWidgetMSN::validateParameterValues()
{
    return true;
}

QString MainOptionsWidgetMSN::defaultDisplayName() const
{
    // FIXME Move the class Dictionary in common internals and use string from
    //       Dictionary::instance()->string(QLatin1String("msn"))
    return i18n("Windows Live Messenger");
}
