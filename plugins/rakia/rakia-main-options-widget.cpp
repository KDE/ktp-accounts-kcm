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

#include "rakia-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

#include <KUser>

RakiaMainOptionsWidget::RakiaMainOptionsWidget(ParameterEditModel *model, QWidget *parent)
    : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::RakiaMainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("account"),  QVariant::String, m_ui->accountLineEdit,  m_ui->accountLabel);
    handleParameter(QLatin1String("password"), QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);
    handleParameter(QLatin1String("alias"),    QVariant::String, m_ui->aliasLineEdit,    m_ui->aliasLabel);

    // if the account is empty on startup we add a new account, therefore
    // set the alias to the current users full name
    if (m_ui->accountLineEdit->text().isEmpty()) {
        KUser user = KUser();
        m_ui->aliasLineEdit->setText(user.property(KUser::FullName).toString());
    }
}

RakiaMainOptionsWidget::~RakiaMainOptionsWidget()
{
    delete m_ui;
}
