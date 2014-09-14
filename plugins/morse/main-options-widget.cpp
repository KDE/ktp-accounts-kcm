/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2014 David Edmundson <davidedmundson@kde.org>
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

#include <klocalizedstring.h>

MainOptionsWidget::MainOptionsWidget(ParameterEditModel *model,
                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::MainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("account"), QVariant::String, m_ui->accountLineEdit, m_ui->accountLabel);

    QTimer::singleShot(0, m_ui->accountLineEdit, SLOT(setFocus()));
}

MainOptionsWidget::~MainOptionsWidget()
{
    delete m_ui;
}

QString MainOptionsWidget::defaultDisplayName() const
{
    return i18nc("name of the IM service Telegram", "Telegram (%1)", m_ui->accountLineEdit->text());
}

#include "main-options-widget.moc"
