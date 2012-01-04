/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "parameter-edit-widget.h"

#include "parameter-edit-delegate.h"
#include "parameter-edit-model.h"

#include "ui_parameter-edit-widget.h"

class ParameterEditWidget::Private
{
public:
    Private()
     : ui(0), delegate(0)
    {
    }

    Ui::ParameterEditWidget *ui;
    ParameterEditDelegate *delegate;
};

ParameterEditWidget::ParameterEditWidget(ParameterEditModel *parameterModel,
                                         QWidget *parent)
 : AbstractAccountParametersWidget(parameterModel, parent),
   d(new Private)
{
    // Set up the UI.
    d->ui = new Ui::ParameterEditWidget;
    d->ui->setupUi(this);

    d->ui->parameterListView->setModel(parameterModel);
    d->delegate = new ParameterEditDelegate(d->ui->parameterListView, this);
    d->ui->parameterListView->setItemDelegate(d->delegate);

    connect(d->delegate,
            SIGNAL(dataChanged(QModelIndex,QVariant,int)),
            SLOT(onDelegateDataChanged(QModelIndex,QVariant,int)));

    Tp::ProtocolParameter foundParameter = parameterModel->parameter(QLatin1String("account"));
    if (foundParameter.isValid()) {
        connect(d->delegate,
                SIGNAL(dataChanged(QModelIndex,QVariant,int)),
                SLOT(updateDefaultDisplayName()));
    }
}

ParameterEditWidget::~ParameterEditWidget()
{
    delete d->ui;
    delete d;
}

void ParameterEditWidget::onDelegateDataChanged(const QModelIndex &index, const QVariant &value, int role)
{
    parameterModel()->setData(index, value, role);
}

void ParameterEditWidget::updateDefaultDisplayName()
{
    Tp::ProtocolParameter foundParameter = parameterModel()->parameter(QLatin1String("account"));
    if (foundParameter.isValid()) {
        setDefaultDisplayName(parameterModel()->data(parameterModel()->indexForParameter(foundParameter)).toString());
    }
}

#include "parameter-edit-widget.moc"
