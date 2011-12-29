/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Lasse Liehu <lliehu@kolumbus.fi>
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

#include "advanced-options-widget.h"

#include <KDebug>

AdvancedOptionsWidget::AdvancedOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent)
         : AbstractAccountParametersWidget(model, parent)
{
    kDebug();

    // Set up the UI.
    m_ui = new Ui::AdvancedOptionsWidget;
    m_ui->setupUi(this);

    handleParameter("port", QVariant::UInt, m_ui->portSpinBox, m_ui->portLabel);
    handleParameter("username", QVariant::String, m_ui->usernameLineEdit, m_ui->usernameLabel);
    handleParameter("password", QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);
    handleParameter("use-ssl", QVariant::Bool, m_ui->useSslCheckBox, m_ui->useSslLabel);
    handleParameter("charset", QVariant::String, m_ui->charsetLineEdit, m_ui->charsetLabel);
}

AdvancedOptionsWidget::~AdvancedOptionsWidget()
{
    kDebug();

    delete m_ui;
}

#include "advanced-options-widget.moc"
