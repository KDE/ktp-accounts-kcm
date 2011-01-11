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

#include <KCMTelepathyAccounts/ProtocolParameterValue>

#include <KDebug>
#include <KMessageBox>
#include <QVariant>
#include <QCheckBox>
#include <QComboBox>

typedef QMap<Tp::ProtocolParameter, QWidget*> ParametersWidgetsMap;

class IcqMainOptionsWidget::Private
{

public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    ParametersWidgetsMap parameters;
    
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

    // Set up the UI.
    d->ui = new Ui::IcqMainOptionsWidget;
    d->ui->setupUi(this);

    handleParameter(parameters, "account", QVariant::String, d->ui->accountLineEdit, d->ui->accountLabel);
    handleParameter(parameters, "password", QVariant::String, d->ui->passwordLineEdit, d->ui->passwordLabel);

    // Prefill UI elements if appropriate.
    prefillUI(values);
}

IcqMainOptionsWidget::~IcqMainOptionsWidget()
{
    kDebug();

    delete d;
}

ParametersWidgetsMap* IcqMainOptionsWidget::internalParametersWidgetsMap() const
{
    return &(d->parameters);
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

