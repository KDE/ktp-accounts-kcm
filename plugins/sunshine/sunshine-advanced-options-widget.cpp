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

#include "sunshine-advanced-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

SunshineAdvancedOptionsWidget::SunshineAdvancedOptionsWidget(ParameterEditModel* model, QWidget* parent)
    : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SunshineAdvancedOptionsWidget;
    m_ui->setupUi(this);

    handleParameter("use-specified-server", QVariant::Bool,
                    m_ui->serverGroupBox,
                    0);
    handleParameter("server", QVariant::String,
                    m_ui->serverLineEdit,
                    m_ui->serverLabel);
    handleParameter("port", QVariant::UInt,
                    m_ui->portSpinBox,
                    m_ui->portLabel);
    handleParameter("use-ssl", QVariant::Bool,
                    m_ui->sslCheckBox,
                    m_ui->sslLabel);
    handleParameter("export-contacts", QVariant::Bool,
                    m_ui->exportContactsCheckBox,
                    m_ui->exportContactsLabel);
}

SunshineAdvancedOptionsWidget::~SunshineAdvancedOptionsWidget()
{
    delete m_ui;
}

#include "sunshine-advanced-options-widget.moc"
