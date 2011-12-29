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

#include "aim-server-settings-widget.h"

AimServerSettingsWidget::AimServerSettingsWidget(ParameterEditModel *model,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::AimServerSettingsWidget;
    m_ui->setupUi(this);

    //server
    handleParameter( "server" , QVariant::String ,m_ui->serverAddress ,m_ui->serverAddressLabel);
    handleParameter( "port" , QVariant::UInt ,m_ui->serverPort ,m_ui->serverPortLabel);
    //security
    handleParameter( "encryption" , QVariant::String ,m_ui->encryption ,m_ui->encryptionLabel);
    handleParameter( "always-use-rv-proxy" , QVariant::Bool ,m_ui->rvProxy ,0);
    //other
    handleParameter( "use-clientlogin" , QVariant::Bool ,m_ui->clientLogin ,0);
    handleParameter( "allow-multiple-logins" , QVariant::Bool ,m_ui->multipleLogins ,0);
}

AimServerSettingsWidget::~AimServerSettingsWidget()
{
    delete m_ui;
}
