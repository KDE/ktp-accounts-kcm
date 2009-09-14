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

#include "integer-edit.h"

#include <KDebug>

#include <QtGui/QIntValidator>

IntegerEdit::IntegerEdit(QWidget *parent)
 : QLineEdit(parent)
{
    connect(this, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));

    // Set the validator range to the range of values in a 32 bit integer (dbus-type 'i').
    setValidator(new QIntValidator(-2147483648, 2147483647, this));
}

IntegerEdit::~IntegerEdit()
{

}

qint32 IntegerEdit::value() const
{
    return text().toInt();
}

void IntegerEdit::setValue(qint32 integer)
{
    setText(QString::number(integer));
}

QValidator::State IntegerEdit::validity() const
{
    int cursorPos = cursorPosition();
    QString txt = text();
    return validator()->validate(txt, cursorPos);
}

QPair<qint32, qint32> IntegerEdit::validRange() const
{
    QPair<qint32, qint32> ret;
    ret.first = 0;
    ret.second = 0;

    const QIntValidator *intValidator = qobject_cast<const QIntValidator*>(validator());

    if (!intValidator) {
        kWarning() << "Somehow this is not an int validator :/";
        return ret;
    }

    ret.first = intValidator->bottom();
    ret.second = intValidator->top();

    return ret;
}

void IntegerEdit::setValidRange(qint32 minimum, qint32 maximum)
{
    setValidator(new QIntValidator(static_cast<int>(minimum), static_cast<int>(maximum), this));
}

void IntegerEdit::onTextChanged(const QString &text)
{
    Q_EMIT integerChanged(text.toInt());
}


#include "integer-edit.moc"

