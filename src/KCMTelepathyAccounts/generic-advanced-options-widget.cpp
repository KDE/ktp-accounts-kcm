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

#include <KDebug>
#include <KTabWidget>
#include <KLocale>
#include <QHBoxLayout>


#include "protocol-parameter-value.h"

#include "generic-advanced-options-widget.h"


class GenericAdvancedOptionsWidget::Private
{
public:
    Private()
    {
        kDebug();
    }
    KTabWidget *tabWidget;
    QList<AbstractAccountParametersWidget*> widgets;
};

GenericAdvancedOptionsWidget::GenericAdvancedOptionsWidget(ParameterEditModel *parameterModel,
                                                           QWidget *parent)
 : AbstractAccountParametersWidget(parameterModel, parent),
   d(new Private)
{
    d->tabWidget = new KTabWidget();
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(d->tabWidget);

    d->tabWidget->setTabBarHidden(true);
}

GenericAdvancedOptionsWidget::~GenericAdvancedOptionsWidget()
{
    kDebug();

    delete d;
}

QList<ProtocolParameterValue> GenericAdvancedOptionsWidget::parameterValues() const
{
    kDebug();

    // the server parameter values
    QList<ProtocolParameterValue> parameters;
    foreach(AbstractAccountParametersWidget *widget, d->widgets)
    {
        kDebug() << "Got these values for storing...";
        foreach(ProtocolParameterValue value, widget->parameterValues())
        {
            kDebug() << value.name() << value.value();
        }
        parameters.append(widget->parameterValues());
    }

    return parameters;
}

bool GenericAdvancedOptionsWidget::validateParameterValues()
{
    kDebug();

    // validate one tab at a time so that the user is not flooded with dialogs
    foreach(AbstractAccountParametersWidget *widget, d->widgets)
    {
        if (!widget->validateParameterValues())
        return false;
    }

    return true;
}

void GenericAdvancedOptionsWidget::setTabBarHidden(bool hidden)
{
    d->tabWidget->setTabBarHidden(hidden);
}

void GenericAdvancedOptionsWidget::addTab(AbstractAccountParametersWidget* tab, const QString& label)
{
    if(d->tabWidget->count() == 1)
    {
        d->tabWidget->setTabBarHidden(false);
    }

    d->tabWidget->addTab(tab, label);
    d->widgets.append(tab);
}


#include "generic-advanced-options-widget.moc"

