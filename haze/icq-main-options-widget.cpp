/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#include "icq-main-options-widget.h"

#include "ui_icq-main-options-widget.h"

#include <KDebug>
#include <KMessageBox>
#include <QVariant>
#include <QCheckBox>
#include <QComboBox>

class IcqMainOptionsWidget::Private
{

public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    Ui::IcqMainOptionsWidget *ui;
};

IcqMainOptionsWidget::IcqMainOptionsWidget(ParameterEditModel *model,
                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent),
   d(new Private)
{
    kDebug();

    // Set up the UI.
    d->ui = new Ui::IcqMainOptionsWidget;
    d->ui->setupUi(this);

    handleParameter("account", QVariant::String, d->ui->accountLineEdit, d->ui->accountLabel);
    handleParameter("password", QVariant::String, d->ui->passwordLineEdit, d->ui->passwordLabel);
}

IcqMainOptionsWidget::~IcqMainOptionsWidget()
{
    kDebug();

    delete d;
}

#include "icq-main-options-widget.moc"

