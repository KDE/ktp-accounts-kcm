/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Thomas Richard
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

#include "abstract-account-parameters-widget.h"
#include "protocol-parameter-value.h"
#include "parameter-edit-model.h"

#include <KDebug>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDataWidgetMapper>


class AbstractAccountParametersWidget::Private
{
public:
    Private()
        : model(0),
        mapper(0)
    {
        kDebug();
    }
    ParameterEditModel *model;
    QDataWidgetMapper *mapper;
    Tp::ProtocolParameterList parameters;
};

AbstractAccountParametersWidget::AbstractAccountParametersWidget(ParameterEditModel *model,
                                                                 QWidget *parent)
        : QWidget(parent),
          d(new Private)
{
    kDebug();

    d->model = model;

    d->mapper = new QDataWidgetMapper(this);
    d->mapper->setModel(d->model);

    d->mapper->setOrientation(Qt::Vertical);
}

AbstractAccountParametersWidget::~AbstractAccountParametersWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> AbstractAccountParametersWidget::parameterValues() const

{
    kDebug();

    return d->model->parameterValues();
}

Tp::ProtocolParameterList AbstractAccountParametersWidget::parameters() const
{
    return d->parameters;
}

bool AbstractAccountParametersWidget::validateParameterValues()
{
    return d->model->validateParameterValues();
}

void AbstractAccountParametersWidget::handleParameter(const QString &parameterName,
                                           QVariant::Type parameterType,
                                           QWidget* dataWidget,
                                           QWidget* labelWidget = 0)
{
    kDebug();

    QList<QWidget*> labelWidgets;
    labelWidgets << labelWidget;
    handleParameter(parameterName, parameterType, dataWidget, labelWidgets);
}

void AbstractAccountParametersWidget::handleParameter(const QString &parameterName,
                                           QVariant::Type parameterType,
                                           QWidget* dataWidget,
                                           QList<QWidget*> labelWidgets)
{
    kDebug() << parameterType << parameterName;

    Tp::ProtocolParameter foundParameter;
    foreach(const Tp::ProtocolParameter &parameter, model()->parameters())
    {
        if ((parameter.name() == parameterName) && (parameter.type() == parameterType)) {
            foundParameter = parameter;
        }
    }

    if(!foundParameter.isValid())
    {
        // hide widgets because they are not needed
        dataWidget->hide();
        foreach(QWidget *label, labelWidgets)
        {
            if(label)
            {
                label->hide();
            }
        }
        return;
    }
    
    QModelIndex index = d->model->indexForParameter(foundParameter);
    if(index.isValid())
    {
        kDebug() << index << parameterName;
        // insert it to valid parameters list
        d->mapper->addMapping(dataWidget, index.row());
        d->mapper->toFirst();
    }
}

ParameterEditModel* AbstractAccountParametersWidget::model() const
{
    return d->model;
}


#include "abstract-account-parameters-widget.moc"

