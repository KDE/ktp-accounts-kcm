/*
 * This file is part of telepathy-firstnames-kcm-plugins
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

#include "salut-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

#include <KDebug>
#include <KUser>

SalutMainOptionsWidget::SalutMainOptionsWidget(ParameterEditModel *model, QWidget *parent)
    : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SalutMainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter("first-name", QVariant::String, m_ui->firstnameLineEdit, m_ui->firstnameLabel);
    handleParameter("last-name",  QVariant::String, m_ui->lastnameLineEdit,  m_ui->lastnameLabel);
    handleParameter("nickname",   QVariant::String, m_ui->nicknameLineEdit,  m_ui->nicknameLabel);

    // if the first- and last-name are empty on startup we add them based on
    // the current users full name
    if(m_ui->firstnameLineEdit->text().isEmpty()) {
        KUser user = KUser();
        m_ui->firstnameLineEdit->setText(user.property(KUser::FullName).toString());
        m_ui->nicknameLineEdit->setText(user.loginName());
    }

    QTimer::singleShot(0, m_ui->firstnameLineEdit, SLOT(setFocus()));
}

SalutMainOptionsWidget::~SalutMainOptionsWidget()
{
    kDebug();

    delete m_ui;
}
