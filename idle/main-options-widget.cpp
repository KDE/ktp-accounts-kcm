/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

MainOptionsWidget::MainOptionsWidget(Tp::ProtocolParameterList parameters,
        const QVariantMap &values,
        QWidget *parent)
         : AbstractAccountParametersWidget(parameters, values, parent),
           d(new Private)
{
    kDebug();

    // Store the parameters this widget supports
    foreach (const Tp::ProtocolParameter &parameter, parameters) {
        if ((parameter.name() == "account") && (parameter.type() == QVariant::String)) {
           d->accountParameter = parameter;
        } else if ((parameter.name() == "fullname") && (parameter.type() == QVariant::String)) {
          d->fullnameParameter = parameter;
        } else if ((parameter.name() == "server") && (parameter.type() == QVariant::String)) {
          d->serverParameter = parameter;
        }
    }

    // Set up the UI.
    d->ui = new Ui::MainOptionsWidget;
    d->ui->setupUi(this);

   // Prefill UI elements if appropriate.
   if (d->accountParameter.isValid()) {
       if (values.contains(d->accountParameter.name())) {
           d->ui->accountLineEdit->setText(values.value(d->accountParameter.name()).toString());
       } else {
           d->ui->accountLineEdit->setText(d->accountParameter.defaultValue().toString());
       }
   }

   if (d->fullnameParameter.isValid()) {
       if (values.contains(d->fullnameParameter.name())) {
           d->ui->fullnameLineEdit->setText(values.value(d->fullnameParameter.name()).toString());
       } else {
           d->ui->fullnameLineEdit->setText(d->fullnameParameter.defaultValue().toString());
       }
   }

   if (d->serverParameter.isValid()) {
       if (values.contains(d->serverParameter.name())) {
           d->ui->serverLineEdit->setText(values.value(d->serverParameter.name()).toString());
       } else {
           d->ui->serverLineEdit->setText(d->serverParameter.defaultValue().toString());
       }
   }

//    Hide any elements we don't have the parameters passed to show.
   if (!d->accountParameter.isValid()) {
       d->ui->accountLabel->hide();
       d->ui->accountLineEdit->hide();
   }

   if (!d->fullnameParameter.isValid()) {
       d->ui->fullnameLabel->hide();
       d->ui->fullnameLineEdit->hide();
   }

   if (!d->serverParameter.isValid()) {
       d->ui->serverLabel->hide();
       d->ui->serverLineEdit->hide();
   }
}

MainOptionsWidget::~MainOptionsWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> MainOptionsWidget::parameterValues() const
{
    kDebug();

    QList<ProtocolParameterValue> parameters;

    // Populate the map of parameters and their values with all the parameters this widget contains.
    if (d->accountParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->accountParameter, d->ui->accountLineEdit->text()));
    }

    if (d->fullnameParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->fullnameParameter, d->ui->fullnameLineEdit->text()));
    }

    if (d->serverParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->serverParameter, d->ui->serverLineEdit->text()));
    }

    return parameters;
}

bool MainOptionsWidget::validateParameterValues()
{
    kDebug();

    if ((!d->ui->accountLineEdit->text().isEmpty()) && (!d->ui->serverLineEdit->text().isEmpty()))
    {
        return true;
    }
    else return false;
}

#include "main-options-widget.moc"
