/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2013 Anant Kamath <kamathanant@gmail.com>
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


#include "main-options-widget.h"

#include <QtGui/QDataWidgetMapper>

#include <ModemManagerQt/manager.h>
#include <ModemManagerQt/modemgsmcardinterface.h>

#include <KCMTelepathyAccounts/ParameterEditModel>

#include "modem-combobox.h"

MainOptionsWidget::MainOptionsWidget(ParameterEditModel *model,
                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    m_ui = new Ui::MainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("number"), QVariant::String, m_ui->numberLineEdit, m_ui->numberLabel);
    handleParameter(QLatin1String("simIdentifier"), QVariant::String, m_ui->comboBox, m_ui->modemLabel, "selectedSimIdentifier");
    QTimer::singleShot(0, m_ui->numberLineEdit, SLOT(setFocus()));
}

MainOptionsWidget::~MainOptionsWidget()
{
    delete m_ui;
}

QString MainOptionsWidget::defaultDisplayName() const
{
    return parameterModel()->data(parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("number")))).toString();
}

#include "main-options-widget.moc"
