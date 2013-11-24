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

#include "sipe-advanced-settings-widget.h"

SipeAdvancedSettingsWidget::SipeAdvancedSettingsWidget(ParameterEditModel *model,
                                                                 QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SipeAdvancedSettingsWidget;
    m_ui->setupUi(this);

    //server
    handleParameter(QLatin1String("login"), QVariant::String, m_ui->loginLineEdit, m_ui->loginLabel);
    handleParameter(QLatin1String("server"), QVariant::String, m_ui->serverLineEdit, m_ui->serverLabel);
    handleParameter(QLatin1String("transport"), QVariant::String, m_ui->transportLineEdit, m_ui->transportLabel);
    handleParameter(QLatin1String("useragent"), QVariant::String, m_ui->useragentLineEdit, m_ui->useragentLabel);
    handleParameter(QLatin1String("authentication"), QVariant::String, m_ui->authenticationLineEdit, m_ui->authenticationLabel);
    handleParameter(QLatin1String("dont-publish"), QVariant::Bool, m_ui->dontpublishCheckBox, 0);
    handleParameter(QLatin1String("email-url"), QVariant::String, m_ui->emailurlLineEdit, m_ui->emailurlLabel);
    handleParameter(QLatin1String("email"), QVariant::String, m_ui->emailLineEdit, m_ui->emailLabel);
    handleParameter(QLatin1String("email-login"), QVariant::String, m_ui->emailloginLineEdit, m_ui->emailloginLabel);
    handleParameter(QLatin1String("email-password"), QVariant::String, m_ui->emailpasswordLineEdit, m_ui->emailpasswordLabel);
    handleParameter(QLatin1String("groupchat-user"), QVariant::String, m_ui->groupchatuserLineEdit, m_ui->groupchatuserLabel);
}

SipeAdvancedSettingsWidget::~SipeAdvancedSettingsWidget()
{
    delete m_ui;
}

#include "sipe-advanced-settings-widget.moc"
