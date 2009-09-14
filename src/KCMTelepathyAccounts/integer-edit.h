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

#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_INTEGER_EDIT_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_INTEGER_EDIT_H

#include <kdemacros.h>

#include <QtCore/QPair>
#include <QtGui/QLineEdit>
#include <QtGui/QValidator>

class KDE_EXPORT IntegerEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit IntegerEdit(QWidget *parent = 0);
    virtual ~IntegerEdit();

    qint32 value() const;
    void setValue(qint32 integer);

    QValidator::State validity() const;

    QPair<qint32, qint32> validRange() const;
    void setValidRange(qint32 minimum, qint32 maximum);

Q_SIGNALS:
    void integerChanged(qint32 integer);

private Q_SLOTS:
    void onTextChanged(const QString &text);
};


#endif // header guard

