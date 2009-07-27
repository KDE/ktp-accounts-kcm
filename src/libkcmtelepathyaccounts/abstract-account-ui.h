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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_UI_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_ABSTRACT_ACCOUNT_UI_H

#include <kdemacros.h>

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QVariant>

class KDE_EXPORT AbstractAccountUi : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractAccountUi);

public:
    AbstractAccountUi(QObject *parent = 0);
    virtual ~AbstractAccountUi();

    virtual const QMap<QString, QVariant::Type> &supportedMandatoryParameters() const;
    virtual const QMap<QString, QVariant::Type> &supportedOptionalParameters() const;

protected:
    virtual void registerSupportedMandatoryParameter(const QString &name, QVariant::Type type);
    virtual void registerSupportedOptionalParameter(const QString &name, QVariant::Type type);

private:
    class Private;
    Private * const d;
};


#endif // header guard

