/*
 * This file is part of telepathy-accounts-kcm
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

#include "myspaceim-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

MySpaceIMMainOptionsWidget::MySpaceIMMainOptionsWidget(ParameterEditModel *model, QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::MySpaceIMMainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("account"),  QVariant::String, m_ui->accountLineEdit,  m_ui->accountLabel, true);
    handleParameter(QLatin1String("password"), QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);
}

MySpaceIMMainOptionsWidget::~MySpaceIMMainOptionsWidget()
{
    delete m_ui;
}

void MySpaceIMMainOptionsWidget::updateDefaultDisplayName()
{
    setDefaultDisplayName(m_ui->accountLineEdit->text());
}

#include "myspaceim-main-options-widget.moc"
