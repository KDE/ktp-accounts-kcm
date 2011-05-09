/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "server-settings-widget.h"

#include <KDebug>

ServerSettingsWidget::ServerSettingsWidget(ParameterEditModel *model,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    kDebug();

    // Set up the UI.
    m_ui = new Ui::ServerSettingsWidget;
    m_ui->setupUi(this);

    handleParameter("server", QVariant::String, m_ui->serverLineEdit, m_ui->serverLabel);
    handleParameter("port", QVariant::UInt, m_ui->portSpinBox, m_ui->portLabel);
    handleParameter("keepalive-interval", QVariant::UInt, m_ui->keepaliveIntervalSpinBox, 0);
    handleParameter("low-bandwidth", QVariant::Bool, m_ui->lowBandwidthCheckBox, 0);
    handleParameter("require-encryption", QVariant::Bool, m_ui->requireEncryptionCheckBox, 0);
    handleParameter("ignore-ssl-errors", QVariant::Bool, m_ui->ignoreSslErrorsCheckBox, 0);
    handleParameter("old-ssl", QVariant::Bool, m_ui->oldSslCheckBox, 0);
    handleParameter("resource", QVariant::String, m_ui->resourceLineEdit, m_ui->resourceLabel);
}

ServerSettingsWidget::~ServerSettingsWidget()
{
    kDebug();

    delete m_ui;
}


#include "server-settings-widget.moc"

