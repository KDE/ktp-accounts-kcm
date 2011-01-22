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

#include "server-settings-widget.h"

#include "ui_server-settings-widget.h"

#include <KDebug>
#include <KMessageBox>


class ServerSettingsWidget::Private
{
public:
    Private()
            : ui(0)
    {
        kDebug();
    }

    Ui::ServerSettingsWidget *ui;
};

ServerSettingsWidget::ServerSettingsWidget(ParameterEditModel *model,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent),
   d(new Private)
{
    kDebug();

    // Set up the UI.
    d->ui = new Ui::ServerSettingsWidget;
    d->ui->setupUi(this);

    handleParameter("server", QVariant::String, d->ui->serverLineEdit, d->ui->serverLabel);
    handleParameter("port", QVariant::UInt, d->ui->portSpinBox, d->ui->portLabel);
    handleParameter("keepalive-interval", QVariant::UInt, d->ui->keepaliveIntervalSpinBox, 0);
    handleParameter("low-bandwidth", QVariant::Bool, d->ui->lowBandwidthCheckBox, 0);
    handleParameter("require-encryption", QVariant::Bool, d->ui->requireEncryptionCheckBox, 0);
    handleParameter("ignore-ssl-errors", QVariant::Bool, d->ui->ignoreSslErrorsCheckBox, 0);
    handleParameter("old-ssl", QVariant::Bool, d->ui->oldSslCheckBox, 0);
}

ServerSettingsWidget::~ServerSettingsWidget()
{
    kDebug();

    delete d;
}


#include "server-settings-widget.moc"

