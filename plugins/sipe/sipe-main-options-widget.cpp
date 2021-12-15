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

#include "sipe-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

SipeMainOptionsWidget::SipeMainOptionsWidget(ParameterEditModel *model, QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SipeMainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("account"),  QVariant::String, m_ui->accountLineEdit,  m_ui->accountLabel);
    handleParameter(QLatin1String("password"), QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);

    // telepathy-sipe require login, but it just equals account in many cases, so
    // let login will be optional (empty) in kcm when it equals to account
    Tp::ProtocolParameter loginParameter = parameterModel()->parameter(QLatin1String("login"));
    Tp::ProtocolParameter accountParameter = parameterModel()->parameter(QLatin1String("account"));
    QString login = parameterModel()->indexForParameter(loginParameter).data().toString();
    QString account = parameterModel()->indexForParameter(accountParameter).data().toString();
    if (account==login) {
        QModelIndex index = parameterModel()->indexForParameter(loginParameter);
        QString empty(QLatin1String(""));
        parameterModel()->setData(index, empty, Qt::EditRole);
    }
}

SipeMainOptionsWidget::~SipeMainOptionsWidget()
{
    delete m_ui;
}

QString SipeMainOptionsWidget::defaultDisplayName() const
{
    return m_ui->accountLineEdit->text();
}

void SipeMainOptionsWidget::submit()
{
    AbstractAccountParametersWidget::submit();

    // if login is empty, copy account value to login
    Tp::ProtocolParameter loginParameter = parameterModel()->parameter(QLatin1String("login"));
    if (parameterModel()->indexForParameter(loginParameter).data().toString().isEmpty()) {
        QString account = parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("account"))).data().toString();
        QModelIndex index = parameterModel()->indexForParameter(loginParameter);
        parameterModel()->setData(index, account, Qt::EditRole);
    }
}
