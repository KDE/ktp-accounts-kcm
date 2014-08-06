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

#include "unsigned-integer-edit.h"

#include <QtGui/QIntValidator>
#include <QDebug>

UnsignedIntegerEdit::UnsignedIntegerEdit(QWidget *parent)
 : QLineEdit(parent)
{
    connect(this, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));

    // Set the validator range to the range of values in a 32 bit unsigned integer (dbus-type 'u').
    // FIXME: Maximum value must be a valid type "int" for the validator to work. What a POS
    setValidator(new QIntValidator(0, 2147483647, this));
}

UnsignedIntegerEdit::~UnsignedIntegerEdit()
{

}

uint UnsignedIntegerEdit::value() const
{
    return text().toUInt();
}

void UnsignedIntegerEdit::setValue(uint unsignedInteger)
{
    setText(QString::number(unsignedInteger));
}

QValidator::State UnsignedIntegerEdit::validity() const
{
    int cursorPos = cursorPosition();
    QString txt = text();
    return validator()->validate(txt, cursorPos);
}

QPair<quint32, quint32> UnsignedIntegerEdit::validRange() const
{
    QPair<quint32, quint32> ret;
    ret.first = 0;
    ret.second = 0;

    QIntValidator const *intValidator = qobject_cast<const QIntValidator*>(validator());

    if (!intValidator) {
        qWarning() << "Somehow this is not an int validator :/";
        return ret;
    }

    ret.first = intValidator->bottom();
    ret.second = intValidator->top();

    return ret;
}

// WARNING: Don't set a range outside of that supported by "int"!!!
void UnsignedIntegerEdit::setValidRange(quint32 minimum, quint32 maximum)
{
    setValidator(new QIntValidator(static_cast<int>(minimum), static_cast<int>(maximum), this));
}

void UnsignedIntegerEdit::onTextChanged(const QString &text)
{
    Q_EMIT unsignedIntegerChanged(text.toUInt());
}


#include "unsigned-integer-edit.moc"

