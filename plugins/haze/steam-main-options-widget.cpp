/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2013 Daniel Cohen <analoguecolour@gmail.com>
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

#include "steam-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

SteamMainOptionsWidget::SteamMainOptionsWidget(ParameterEditModel *model, QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SteamMainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("account"),  QVariant::String, m_ui->accountLineEdit,  m_ui->accountLabel);
    handleParameter(QLatin1String("steam-guard-code"),  QVariant::String, m_ui->steamguardEdit,  m_ui->steamguardLabel);
    handleParameter(QLatin1String("password"), QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);
}

SteamMainOptionsWidget::~SteamMainOptionsWidget()
{
    delete m_ui;
}

QString SteamMainOptionsWidget::defaultDisplayName() const
{
    return m_ui->accountLineEdit->text();
}
