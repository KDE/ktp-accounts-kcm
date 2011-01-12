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

#include <KDebug>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>


class AbstractAccountParametersWidget::Private
{
public:
    Private()
    {
        kDebug();
    }

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

    Q_UNUSED(values);
}

AbstractAccountParametersWidget::~AbstractAccountParametersWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> AbstractAccountParametersWidget::parameterValues() const
{
    kDebug();

    QList<ProtocolParameterValue> parameters;

    ParametersWidgetsMap::const_iterator i = internalParametersWidgetsMap()->constBegin();
    while (i != internalParametersWidgetsMap()->constEnd()) {

        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(i.value());
        if(lineEdit)
        {
            parameters.append(ProtocolParameterValue(i.key(), lineEdit->text()));
            ++i;
            continue;
        }

        QCheckBox *checkBox = qobject_cast<QCheckBox*>(i.value());
        if(checkBox)
        {
            parameters.append(ProtocolParameterValue(i.key(), checkBox->isChecked()));
            ++i;
            continue;
        }

        QComboBox *comboBox = qobject_cast<QComboBox*>(i.value());
        if(comboBox)
        {
            parameters.append(ProtocolParameterValue(i.key(), comboBox->currentText()));
            ++i;
            continue;
        }

        QSpinBox *spinBox = qobject_cast<QSpinBox*>(i.value());
        if(spinBox)
        {
            parameters.append(ProtocolParameterValue(i.key(), spinBox->value()));
            ++i;
            continue;
        }

        kDebug() << "WIDGET TYPE NOT SUPPORTED!";
        ++i;
    }

    return parameters;
}

Tp::ProtocolParameterList AbstractAccountParametersWidget::parameters() const
{
    return d->parameters;
}

bool AbstractAccountParametersWidget::validateParameterValues()
{
    return true;
}


ParametersWidgetsMap* AbstractAccountParametersWidget::internalParametersWidgetsMap() const
{
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

    if(foundParameter.isValid())
    {
        // insert it to valid parameters list
        internalParametersWidgetsMap()->insert(foundParameter, dataWidget);
    }
    else
    {
        // hide widgets because they are not needed
        dataWidget->hide();
        foreach(QWidget *label, labelWidgets)
        {
            label->hide();
        }
    }


}

void AbstractAccountParametersWidget::prefillUI(const QVariantMap& values)
{
    kDebug();

    ParametersWidgetsMap::const_iterator i = internalParametersWidgetsMap()->constBegin();
    while (i != internalParametersWidgetsMap()->constEnd()) {
        kDebug() << "Search widget for " << i.key().name();
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(i.value());
        if(lineEdit)
        {
            if(values.value(i.key().name()).isValid())
            {
                lineEdit->setText(values.value(i.key().name()).toString());
            }
            else
            {
                lineEdit->setText(i.key().defaultValue().toString());
            }
            ++i;
            continue;
        }

        QCheckBox* checkBox = qobject_cast<QCheckBox*>(i.value());
        if(checkBox)
        {
            if(values.value(i.key().name()).isValid())
            {
                checkBox->setChecked(values.value(i.key().name()).toBool());
            }
            else
            {
                checkBox->setChecked(i.key().defaultValue().toBool());
            }
            ++i;
            continue;
        }

        QComboBox* comboBox = qobject_cast<QComboBox*>(i.value());
        if(checkBox)
        {
            if(values.value(i.key().name()).isValid())
            {
                comboBox->setEditText(values.value(i.key().name()).toString());
            }
            else
            {
                comboBox->setEditText(i.key().defaultValue().toString());
            }
            ++i;
            continue;
        }

        QSpinBox* spinBox = qobject_cast<QSpinBox*>(i.value());
        if(spinBox)
        {
            if(values.value(i.key().name()).isValid())
            {
                spinBox->setValue(values.value(i.key().name()).toInt());
            }
            else
            {                
                spinBox->setValue(i.key().defaultValue().toInt());
            }
            ++i;
            continue;
        }

        kDebug() << "WIDGET TYPE UNKNOWN!";
        ++i;
    }
}

#include "abstract-account-parameters-widget.moc"

