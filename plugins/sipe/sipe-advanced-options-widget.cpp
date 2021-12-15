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

#include "sipe-advanced-options-widget.h"

SipeAdvancedOptionsWidget::SipeAdvancedOptionsWidget(ParameterEditModel *model, QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SipeAdvancedOptionsWidget;
    m_ui->setupUi(this);

    //server
    handleParameter(QLatin1String("login"), QVariant::String, m_ui->loginLineEdit, m_ui->loginLabel);
    handleParameter(QLatin1String("server"), QVariant::String, m_ui->serverLineEdit, m_ui->serverLabel);
    handleParameter(QLatin1String("port"),   QVariant::UInt, m_ui->portSpinBox, m_ui->portLabel);
    handleParameter(QLatin1String("transport"), QVariant::String, m_ui->transportComboBox, m_ui->transportLabel);
    handleParameter(QLatin1String("useragent"), QVariant::String, m_ui->useragentLineEdit, m_ui->useragentLabel);
    handleParameter(QLatin1String("authentication"), QVariant::String, m_ui->authenticationComboBox, m_ui->authenticationLabel);
    handleParameter(QLatin1String("single-sign-on"), QVariant::Bool, m_ui->singlesignonCheckBox);
    handleParameter(QLatin1String("don't-publish-calendar"), QVariant::Bool, m_ui->dontpublishcalendarCheckBox);
}

SipeAdvancedOptionsWidget::~SipeAdvancedOptionsWidget()
{
    delete m_ui;
}
