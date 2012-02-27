/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "main-options-widget-kde-talk.h"
#include "ui_main-options-widget-kde-talk.h"

#include <KCMTelepathyAccounts/ParameterEditModel>


const QString serverAddress = QLatin1String("kdetalk.net");

MainOptionsWidgetKDETalk::MainOptionsWidgetKDETalk(ParameterEditModel *model,
                                                   QWidget *parent)
    : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::MainOptionsWidgetKDETalk;
    m_ui->setupUi(this);

    Tp::ProtocolParameter parameter = parameterModel()->parameter(QLatin1String("account"));
    QModelIndex index = parameterModel()->indexForParameter(parameter);
    if (index.isValid()) {
        QString account = index.data().toString();
        //strip off any "@kdetalk.net" from the parameter when displaying it in the text edit.
        account = account.left(account.indexOf(QLatin1Char('@')));
        m_ui->accountLineEdit->setText(account);
    }

    handleParameter(QLatin1String("password"), QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);
    QTimer::singleShot(0, m_ui->accountLineEdit, SLOT(setFocus()));
}

MainOptionsWidgetKDETalk::~MainOptionsWidgetKDETalk()
{
    delete m_ui;
}

void MainOptionsWidgetKDETalk::submit()
{
    Tp::ProtocolParameter parameter = parameterModel()->parameter(QLatin1String("account"));
    QModelIndex index = parameterModel()->indexForParameter(parameter);
    if (index.isValid()) {
        QString account = m_ui->accountLineEdit->text();

        //append "@kdetalk.net"
        account.append(QLatin1Char('@'));
        account.append(serverAddress);

        //update the model with the account value from the text box.
        parameterModel()->setData(index, account, Qt::EditRole);
    }
    AbstractAccountParametersWidget::submit();
}

bool MainOptionsWidgetKDETalk::validateParameterValues()
{
    if (m_ui->accountLineEdit->text().isEmpty() || m_ui->passwordLineEdit->text().isEmpty()) {
        return false;
    }

    return true;
}

#include "main-options-widget-kde-talk.moc"
