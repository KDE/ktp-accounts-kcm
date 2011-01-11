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

#include "icq-main-options-widget.h"

#include "ui_icq-main-options-widget.h"

#include <KCMTelepathyAccounts/ProtocolParameterValue>

#include <KDebug>
#include <KMessageBox>


class IcqMainOptionsWidget::Private
{
public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameter accountParameter;
    Tp::ProtocolParameter passwordParameter;

    Ui::IcqMainOptionsWidget *ui;
};

IcqMainOptionsWidget::IcqMainOptionsWidget(Tp::ProtocolParameterList parameters,
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
        } else if ((parameter.name() == "password") && (parameter.type() == QVariant::String)) {
                d->passwordParameter = parameter;
        }
    }

    // Set up the UI.
    d->ui = new Ui::IcqMainOptionsWidget;
    d->ui->setupUi(this);

    // Prefill UI elements if appropriate.
    if (d->accountParameter.isValid()) {
        if (values.contains(d->accountParameter.name())) {
            d->ui->accountLineEdit->setText(values.value(d->accountParameter.name()).toString());
        } else {
            d->ui->accountLineEdit->setText(d->accountParameter.defaultValue().toString());
        }
    }

    if (d->passwordParameter.isValid()) {
        if (values.contains(d->passwordParameter.name())) {
            d->ui->passwordLineEdit->setText(values.value(d->passwordParameter.name()).toString());
        } else {
            d->ui->passwordLineEdit->setText(d->passwordParameter.defaultValue().toString());
        }
    }

    // Hide any elements we don't have the parameters passed to show.
    if (!d->accountParameter.isValid()) {
        d->ui->accountLabel->hide();
        d->ui->accountLineEdit->hide();
    }

    if (!d->passwordParameter.isValid()) {
        d->ui->passwordLabel->hide();
        d->ui->passwordLineEdit->hide();
    }
}

IcqMainOptionsWidget::~IcqMainOptionsWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> IcqMainOptionsWidget::parameterValues() const
{
    kDebug();

    QList<ProtocolParameterValue> parameters;

    // Populate the map of parameters and their values with all the parameters this widget contains.
    if (d->accountParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->accountParameter, d->ui->accountLineEdit->text()));
    }

    if (d->passwordParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->passwordParameter, d->ui->passwordLineEdit->text()));
    }

    return parameters;
}

bool IcqMainOptionsWidget::validateParameterValues()
{
    kDebug();

    // Username is currently the only required parameter
    if (d->ui->accountLineEdit->text().isEmpty()) {
        kDebug() << "Returning false and alerting the user.";

        KMessageBox::error(this, i18n("Please enter an ICQ UIN."));

        return false;
    }

    return true;
}


#include "icq-main-options-widget.moc"

