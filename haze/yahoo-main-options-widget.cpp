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

#include <KDebug>

class YahooMainOptionsWidget::Private
{
public:
    Ui::YahooMainOptionsWidget* ui;

    Private()
	: ui(0)
    {
        kDebug();
    }
};

YahooMainOptionsWidget::YahooMainOptionsWidget(ParameterEditModel* model, QWidget* parent)
        : AbstractAccountParametersWidget(model, parent), d(new Private)
{
    kDebug();

    //setup the Ui
    d->ui = new Ui::YahooMainOptionsWidget;
    d->ui->setupUi(this);

    //map the widgets to their data
    handleParameter("account", QVariant::String, d->ui->accountLineEdit, d->ui->accountLabel);
    handleParameter("password", QVariant::String, d->ui->passwordLineEdit, d->ui->passwordLabel);
}

YahooMainOptionsWidget::~YahooMainOptionsWidget()
{
    kDebug();

    delete d->ui;
    delete d;
}
