/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
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
#include "parameter-edit-model.h"
#include "validated-line-edit.h"

#include <KLocalizedString>
#include <KDebug>

#include <QtCore/QMap>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QDataWidgetMapper>


class AbstractAccountParametersWidget::Private
{
public:
    Private()
        : parameterModel(0),
          mapper(0)
    {
    }
    ParameterEditModel *parameterModel;
    QDataWidgetMapper *mapper;
    QList<QWidget*> mappedWidgets;
    QMap<QPersistentModelIndex,ValidatedLineEdit*> validatedWidgets;
    QString errorMessage;
};

AbstractAccountParametersWidget::AbstractAccountParametersWidget(ParameterEditModel *parameterModel,
                                                                 QWidget *parent)
        : QWidget(parent),
          d(new Private)
{
    d->parameterModel = parameterModel;
    d->errorMessage = i18n("All mandatory fields must be filled");

    d->mapper = new QDataWidgetMapper(this);
    d->mapper->setModel(d->parameterModel);
    d->mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    d->mapper->setOrientation(Qt::Vertical);
}

AbstractAccountParametersWidget::~AbstractAccountParametersWidget()
{
    delete d;
}

QString AbstractAccountParametersWidget::errorMessage() const
{
    return d->errorMessage;
}

void AbstractAccountParametersWidget::setErrorMessage(const QString &errorMsg)
{
    d->errorMessage = errorMsg;
}

bool AbstractAccountParametersWidget::validateParameterValues()
{
    return d->parameterModel->validateParameterValues();
}

void AbstractAccountParametersWidget::handleParameter(const QString &parameterName,
                                           QVariant::Type parameterType,
                                           QWidget* dataWidget,
                                           QWidget* labelWidget)
{
    QList<QWidget*> labelWidgets;
    if (labelWidget) {
        labelWidgets << labelWidget;
    }
    handleParameter(parameterName, parameterType, dataWidget, labelWidgets);
}

void AbstractAccountParametersWidget::handleParameter(const QString &parameterName,
                                           QVariant::Type parameterType,
                                           QWidget* dataWidget,
                                           const QList<QWidget*> &labelWidgets)
{
    kDebug() << parameterType << parameterName;

    Tp::ProtocolParameter foundParameter = d->parameterModel->parameter(parameterName);

    if (!foundParameter.isValid() || foundParameter.type() != parameterType) {
        // hide widgets because they are not needed
        kDebug() << "Field" << parameterName << "hidden";
        dataWidget->hide();
        Q_FOREACH (QWidget *label, labelWidgets) {
            if (label) {
                label->hide();
            }
        }
        return;
    }

    QModelIndex index = d->parameterModel->indexForParameter(foundParameter);
    if (index.isValid()) {
        kDebug() << index << parameterName;
        // insert it to valid parameters list
        //for text edit boxes we force it to use the plainText property so that we don't get HTML all over our options
        if (dataWidget->inherits("QTextEdit")) {
            d->mapper->addMapping(dataWidget, index.row(), "plainText");
        } else if (dataWidget->inherits("QComboBox") && parameterType == QVariant::String) {
            d->mapper->addMapping(dataWidget, index.row(), "currentText");
        } else {
            d->mapper->addMapping(dataWidget, index.row());
        }
        d->mapper->toFirst();

        // check if the passed parameter is a validated one.. If so we're going to set the model here
        ValidatedLineEdit *validated = qobject_cast<ValidatedLineEdit*>(dataWidget);
        if (validated) {
            d->validatedWidgets.insert(index, validated);
        }

        if (!(index.flags() & Qt::ItemIsEnabled)) {
            dataWidget->setEnabled(false);
        }

        d->mappedWidgets.append(dataWidget);
    }
}

void AbstractAccountParametersWidget::submit()
{
    d->mapper->submit();

    QMap<QPersistentModelIndex, ValidatedLineEdit*>::const_iterator i = d->validatedWidgets.constBegin();
    while (i != d->validatedWidgets.constEnd()) {
        d->parameterModel->setData(i.key(), i.value()->validationState(), ParameterEditModel::ValidityRole);
        ++i;
    }

    //reset disabled widgets to their default values
    Q_FOREACH (QWidget *w, d->mappedWidgets) {
        if (!w->isEnabled()) {
            QModelIndex index = d->parameterModel->index(d->mapper->mappedSection(w), 0);
            QVariant defaultValue = d->parameterModel->data(index, ParameterEditModel::DefaultValueRole);
            d->parameterModel->setData(index, defaultValue, Qt::EditRole);
        }
    }
}

ParameterEditModel* AbstractAccountParametersWidget::parameterModel() const
{
    return d->parameterModel;
}

QString AbstractAccountParametersWidget::defaultDisplayName() const
{
    kWarning() << "This method should be implemented by derived classed";
    return QString();
}


#include "abstract-account-parameters-widget.moc"
