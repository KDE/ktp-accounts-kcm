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

#include <KCMTelepathyAccounts/ParameterEditModel>

YahooServerSettingsWidget::YahooServerSettingsWidget(ParameterEditModel *model,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::YahooServerSettingsWidget;
    m_ui->setupUi(this);
    //server
    handleParameter(QLatin1String("port"), QVariant::UInt, m_ui->serverPort, m_ui->serverPortLabel);
    //file transfer
    handleParameter(QLatin1String("xfer-host"), QVariant::String, m_ui->xferHost, m_ui->xferHostLabel);
    handleParameter(QLatin1String("xfer-port"), QVariant::UInt, m_ui->xferPort, m_ui->xferPortLabel);
    //locale
    handleParameter(QLatin1String("room-list-locale"), QVariant::String, m_ui->roomListLocale, m_ui->roomListLocaleLablel);
    handleParameter(QLatin1String("charset"), QVariant::String, m_ui->charsetComboBox, m_ui->charsetLabel);
    //other
    handleParameter(QLatin1String("proxy-ssl"), QVariant::Bool, m_ui->SslProxy);
    handleParameter(QLatin1String("ignore-invites"), QVariant::Bool, m_ui->ignoreInvites);

    // update combo box for charset parameter
    //KF5 TODO
//     m_ui->charsetComboBox->setCurrentItem(parameterModel()->data(
//             parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("charset"))),
//             ParameterEditModel::ValueRole).toString());
}

YahooServerSettingsWidget::~YahooServerSettingsWidget()
{
    delete m_ui;
}

#include "yahoo-server-settings-widget.moc"
