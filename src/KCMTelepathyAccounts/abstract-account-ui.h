/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_UI_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_UI_H

#include "kcm_telepathy_accounts_export.h"

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <TelepathyQt/ConnectionManager>

class AbstractAccountParametersWidget;
class ParameterEditModel;

class KCM_TELEPATHY_ACCOUNTS_EXPORT AbstractAccountUi : public QObject
{
    Q_OBJECT

public:
    explicit AbstractAccountUi(QObject *parent = 0);
    virtual ~AbstractAccountUi();

    virtual const QMap<QString, QVariant::Type> &supportedParameters() const;

    virtual AbstractAccountParametersWidget
              *mainOptionsWidget(ParameterEditModel *parameterModel,
                                 QWidget *parent = 0) const = 0;

    virtual bool hasAdvancedOptionsWidget() const;
    virtual AbstractAccountParametersWidget
              *advancedOptionsWidget(ParameterEditModel *parameterModel,
                                     QWidget *parent = 0) const;

protected:
    virtual void registerSupportedParameter(const QString &name, QVariant::Type type);

private:
    Q_DISABLE_COPY(AbstractAccountUi);

    class Private;
    Private * const d;
};


#endif // header guard

