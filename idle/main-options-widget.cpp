/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Lasse Liehu <lliehu@kolumbus.fi>
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

#include "ui_main-options-widget.h"

#include <KDebug>
#include <KMessageBox>
#include <KCMTelepathyAccounts/ProtocolParameterValue>

class MainOptionsWidget::Private
{
public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameter accountParameter;
    Tp::ProtocolParameter fullnameParameter;
    Tp::ProtocolParameter serverParameter;

    Ui::MainOptionsWidget *ui;
};

MainOptionsWidget::MainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent)
         : AbstractAccountParametersWidget(model, parent),
           d(new Private)
{
    kDebug();

    // Set up the UI.
    d->ui = new Ui::MainOptionsWidget;
    d->ui->setupUi(this);

    handleParameter("account", QVariant::String, d->ui->accountLineEdit, d->ui->accountLabel);
    handleParameter("server", QVariant::String, d->ui->serverLineEdit, d->ui->serverLabel);
    handleParameter("fullname", QVariant::String, d->ui->fullnameLineEdit, d->ui->fullnameLabel);


}

MainOptionsWidget::~MainOptionsWidget()
{
    kDebug();

    delete d;
}

#include "main-options-widget.moc"
