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

#include "advanced-options-widget.h"

#include "ui_advanced-options-widget.h"

#include <KDebug>
#include <KMessageBox>
#include <KCMTelepathyAccounts/ProtocolParameterValue>

class AdvancedOptionsWidget::Private
{
public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameter charsetParameter;
    Tp::ProtocolParameter portParameter;
    Tp::ProtocolParameter passwordParameter;
    Tp::ProtocolParameter useSslParameter;

    Ui::AdvancedOptionsWidget *ui;
};

AdvancedOptionsWidget::AdvancedOptionsWidget(
        Tp::ProtocolParameterList parameters, const QVariantMap &values,
        QWidget *parent)
         : AbstractAccountParametersWidget(parameters, values, parent),
           d(new Private)
{
    kDebug();

    // Store the parameters this widget supports
    foreach (const Tp::ProtocolParameter &parameter, parameters) {
        if ((parameter.name() == "charset") && (parameter.type() == QVariant::String)) {
           d->charsetParameter = parameter;
        } else if ((parameter.name() == "port") && (parameter.type() == QVariant::UInt)) {
           d->portParameter = parameter;
        } else if ((parameter.name() == "password") && (parameter.type() == QVariant::String)) {
           d->passwordParameter = parameter;
        } else if ((parameter.name() == "use-ssl") && (parameter.type() == QVariant::Bool)) {
           d->useSslParameter = parameter;
        }
    }

    // Set up the UI.
    d->ui = new Ui::AdvancedOptionsWidget;
    d->ui->setupUi(this);

   // Prefill UI elements if appropriate.
   if (d->charsetParameter.isValid()) {
       if (values.contains(d->charsetParameter.name())) {
           d->ui->charsetLineEdit->setText(values.value(d->charsetParameter.name()).toString());
       } else {
           d->ui->charsetLineEdit->setText(d->charsetParameter.defaultValue().toString());
       }
   }

   if (d->portParameter.isValid()) {
       if (values.contains(d->portParameter.name())) {
           d->ui->portSpinBox->setValue(values.value(d->portParameter.name()).toUInt());
       } else {
           d->ui->portSpinBox->setValue(d->portParameter.defaultValue().toUInt());
       }
   }

   if (d->useSslParameter.isValid()) {
       if (values.contains(d->useSslParameter.name())) {
           d->ui->useSslCheckBox->setChecked(values.value(d->useSslParameter.name()).toBool());
       } else {
           d->ui->useSslCheckBox->setChecked(d->useSslParameter.defaultValue().toBool());
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
  if (!d->charsetParameter.isValid()) {
       d->ui->charsetLabel->hide();
       d->ui->charsetLineEdit->hide();
   }

  if (!d->portParameter.isValid()) {
       d->ui->portLabel->hide();
       d->ui->portSpinBox->hide();
   }

  if (!d->useSslParameter.isValid()) {
       d->ui->useSslLabel->hide();
       d->ui->useSslCheckBox->hide();
   }

   if (!d->passwordParameter.isValid()) {
       d->ui->passwordLabel->hide();
       d->ui->passwordLineEdit->hide();
   }
}

AdvancedOptionsWidget::~AdvancedOptionsWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> AdvancedOptionsWidget::parameterValues() const
{
    kDebug();

    QList<ProtocolParameterValue> parameters;

    // Populate the map of parameters and their values with all the parameters this widget contains.
    if (d->charsetParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->charsetParameter, d->ui->charsetLineEdit->text()));
    }

    if (d->portParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->portParameter, d->ui->portSpinBox->value()));
    }

    if (d->useSslParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->useSslParameter, d->ui->useSslCheckBox->isChecked()));
    }

    if (d->passwordParameter.isValid()) {
        parameters.append(ProtocolParameterValue(d->passwordParameter, d->ui->passwordLineEdit->text()));
    }

    return parameters;
}

bool AdvancedOptionsWidget::validateParameterValues()
{
    kDebug();


    return true;
}

#include "advanced-options-widget.moc"

