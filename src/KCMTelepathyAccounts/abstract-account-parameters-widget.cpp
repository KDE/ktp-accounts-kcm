/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

AbstractAccountParametersWidget::AbstractAccountParametersWidget(Tp::ProtocolParameterList parameters,
                                                                 const QVariantMap &values,
                                                                 QWidget *parent)
        : QWidget(parent),
          d(new Private)
{
    kDebug();

    d->parameters = parameters;
    d->model = new ParameterEditModel(this);
    d->mapper = new QDataWidgetMapper(this);
    d->mapper->setModel(d->model);
    foreach(const QString &key, values.keys())
    {
        kDebug() << key << values[key];
    }

    // Add the parameters to the model.
    foreach (const Tp::ProtocolParameter &parameter, parameters) {
        kDebug() << "Add Parameter:" << parameter.name() << "default:" << parameter.defaultValue() << values.value(parameter.name()) << values.value(parameter.name(), parameter.defaultValue());
        d->model->addItem(parameter, values.value(parameter.name(), parameter.defaultValue()));
    }
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


ParametersWidgetsMap* AbstractAccountParametersWidget::internalParametersWidgetsMap() const
{
    
    kDebug() << "DEPRECATED";
    ParametersWidgetsMap *map = new ParametersWidgetsMap();
    return map;
}

void AbstractAccountParametersWidget::handleParameter(const Tp::ProtocolParameterList& parameters,
                                           const QString &parameterName,
                                           QVariant::Type parameterType,
                                           QWidget* dataWidget,
                                           QWidget* labelWidget)
{
    kDebug();

    QList<QWidget*> labelWidgets;
    labelWidgets << labelWidget;
    handleParameter(parameters, parameterName, parameterType, dataWidget, labelWidgets);
}

void AbstractAccountParametersWidget::handleParameter(const Tp::ProtocolParameterList& parameters,
                                           const QString &parameterName,
                                           QVariant::Type parameterType,
                                           QWidget* dataWidget,
                                           QList<QWidget*> labelWidgets)
{
    kDebug() << parameterName << parameterType;

    Tp::ProtocolParameter foundParameter;
    foreach(const Tp::ProtocolParameter &parameter, parameters)
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
            label->hide();
        }
        return;
    }
    
    int modelRow = d->model->rowForParameter(foundParameter);
    if(modelRow != -1)
    {
        kDebug() << modelRow << parameterName;
        // insert it to valid parameters list
        d->mapper->addMapping(dataWidget, modelRow);
        d->mapper->toFirst();
    }
}

ParameterEditModel* AbstractAccountParametersWidget::model() const
{
    return d->model;
}


void AbstractAccountParametersWidget::prefillUI(const QVariantMap& values)
{
    kDebug() << "DEPRECATED";
}

#include "abstract-account-parameters-widget.moc"

