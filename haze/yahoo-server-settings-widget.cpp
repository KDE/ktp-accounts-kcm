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

#include "yahoo-server-settings-widget.h"

#include <KDebug>

YahooServerSettingsWidget::YahooServerSettingsWidget(ParameterEditModel *model,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    kDebug();

    // Set up the UI.
    m_ui = new Ui::YahooServerSettingsWidget;
    m_ui->setupUi(this);
    
    //server
    handleParameter("port", QVariant::UInt, m_ui->serverPort, m_ui->serverPortLabel);
    //file transfer
    handleParameter("xfer-host", QVariant::String, m_ui->xferHost, m_ui->xferHostLabel);
    handleParameter("xfer-port", QVariant::UInt, m_ui->xferPort, m_ui->xferPortLabel);
    //locale
    handleParameter("room-list-locale", QVariant::String, m_ui->roomListLocale, m_ui->roomListLocaleLablel);
    handleParameter("charset", QVariant::String, m_ui->charsetComboBox, m_ui->charsetLabel);
    //other
    handleParameter("proxy-ssl", QVariant::Bool, m_ui->SslProxy, 0);
    handleParameter("ignore-invites", QVariant::Bool, m_ui->ignoreInvites, 0);
}

YahooServerSettingsWidget::~YahooServerSettingsWidget()
{
    kDebug();

    delete m_ui;
}


