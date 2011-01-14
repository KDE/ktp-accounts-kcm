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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_PARAMETERS_WIDGET_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_PARAMETERS_WIDGET_H

#include <kdemacros.h>

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtGui/QWidget>

#include <TelepathyQt4/ConnectionManager>

class ParameterEditModel;
class ProtocolParameterValue;
class ParameterEditModel;

typedef QMap<Tp::ProtocolParameter, QWidget*> ParametersWidgetsMap;

class KDE_EXPORT AbstractAccountParametersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractAccountParametersWidget(ParameterEditModel *parameterModel,
                                             const QString &profile,
                                             QWidget *parent = 0);
    virtual ~AbstractAccountParametersWidget();

    virtual QList<ProtocolParameterValue> parameterValues() const;
    virtual Tp::ProtocolParameterList parameters() const;
    virtual bool validateParameterValues();

protected:
    void handleParameter(const QString &parameterName,
                         QVariant::Type parameterType,
                         QWidget *dataWidget,
                         const QList<QWidget*> &labelWidgets);
    void handleParameter(const QString &parameterName,
                         QVariant::Type parameterType,
                         QWidget *dataWidget,
                         QWidget *labelWidget);

    ParameterEditModel *parameterModel() const;
    QString profile() const;

private:
    Q_DISABLE_COPY(AbstractAccountParametersWidget);

    class Private;
    Private * const d;
};


#endif // header guard

