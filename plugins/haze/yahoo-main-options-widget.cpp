/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Lasath Fernando <kde@lasath.org>
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

#include "yahoo-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

#include <KDebug>

YahooMainOptionsWidget::YahooMainOptionsWidget(ParameterEditModel* model, QWidget* parent)
        : AbstractAccountParametersWidget(model, parent)
{
    kDebug() << "Creating Yahoo Account";

    //setup the Ui
    m_ui = new Ui::YahooMainOptionsWidget;
    m_ui->setupUi(this);

    //map the widgets to their data
    handleParameter("account", QVariant::String, m_ui->accountLineEdit, m_ui->accountLabel);
    handleParameter("password", QVariant::String, m_ui->passwordLineEdit, m_ui->passwordLabel);
    QTimer::singleShot(0, m_ui->accountLineEdit, SLOT(setFocus()));
}

YahooMainOptionsWidget::~YahooMainOptionsWidget()
{
    kDebug();

    delete m_ui;
}

void YahooMainOptionsWidget::submit()
{
    Tp::ProtocolParameter parameter = parameterModel()->parameter("account");
    QModelIndex index = parameterModel()->indexForParameter(parameter);
    if (index.isValid()) {
        QString account = m_ui->accountLineEdit->text();

        if (account.contains(QLatin1Char('@'))) {
            int atIndex = account.indexOf(QLatin1Char('@'));

            //update the model with the account value from the text box.
            parameterModel()->setData(index, account.left(atIndex), Qt::EditRole);
        }
    }

    AbstractAccountParametersWidget::submit();
}
