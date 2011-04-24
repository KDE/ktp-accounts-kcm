/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
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

#include "main-options-widget-facebook.h"


#include <KCMTelepathyAccounts/ParameterEditModel>
#include <KDebug>

MainOptionsWidgetFacebook::MainOptionsWidgetFacebook(ParameterEditModel *model,
                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    kDebug();

    // Set up the UI.
    m_ui = new Ui::MainOptionsWidgetFacebook;
    m_ui->setupUi(this);

    // We cannot use handleParameter directly as we need to append @chat.facebook.com onto the end of the JID.
    // Profiles have no method to do this, so pseudo-hardcoding is the only available option.

    Tp::ProtocolParameter parameter = parameterModel()->parameter("account");
    QModelIndex index = parameterModel()->indexForParameter(parameter);
    if (index.isValid()) {
        QString account = index.data().toString();
        //strip off any "@chat.facebook.com" from the parameter when displaying it in the text edit.
        account = account.left(account.indexOf('@'));
        m_ui->accountLineEdit->setText(account);
    }

    handleParameter("password", QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);
}

MainOptionsWidgetFacebook::~MainOptionsWidgetFacebook()
{
    kDebug();

    delete m_ui;
}

void MainOptionsWidgetFacebook::submit()
{
    Tp::ProtocolParameter parameter = parameterModel()->parameter("account");
    QModelIndex index = parameterModel()->indexForParameter(parameter);
    if (index.isValid()) {
        QString account = m_ui->accountLineEdit->text();

        //append "@chat.facebook.com" (fetching the address from the default params for as much future compatiability as possible)
        account.append('@');
        QString serverAddress = parameterModel()->indexForParameter(parameterModel()->parameter("server")).data().toString();
        account.append(serverAddress);

        //update the model with the account value from the text box.
        parameterModel()->setData(index, account, Qt::EditRole);
    }
    AbstractAccountParametersWidget::submit();
}

#include "main-options-widget-facebook.moc"

