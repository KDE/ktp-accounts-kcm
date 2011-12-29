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

#include "salut-advanced-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

SalutAdvancedOptionsWidget::SalutAdvancedOptionsWidget(ParameterEditModel* model, QWidget* parent)
    : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SalutAdvancedOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("published-name"),  QVariant::String, m_ui->pubnameLineEdit, m_ui->pubnameLabel);
    handleParameter(QLatin1String("email"), QVariant::String, m_ui->emailLineEdit, m_ui->emailLabel);
    handleParameter(QLatin1String("jid"),   QVariant::String, m_ui->jidLineEdit,   m_ui->jidLabel);
}

SalutAdvancedOptionsWidget::~SalutAdvancedOptionsWidget()
{
    delete m_ui;
}

#include "salut-advanced-options-widget.moc"
