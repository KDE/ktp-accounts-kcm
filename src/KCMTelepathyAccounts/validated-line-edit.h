/*
 * This file is part of telepathy-accounts-kcm
 *
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

#ifndef VALIDATEDLINEEDIT_H
#define VALIDATEDLINEEDIT_H

#include <KLineEdit>
#include <kdemacros.h>
#include <QtGui/QValidator>

class QLabel;
class QHBoxLayout;
class QToolButton;
class ValidationIconWidget;
class QAbstractItemModel;
class QModelIndex;

class KDE_EXPORT ValidatedLineEdit : public KLineEdit
{
    Q_OBJECT

public:
    enum PredefinedValidator {
        NotEmptyValidator,
        EmailValidator,
        HostnameValidator,
        IPAddressValidator
    };

    ValidatedLineEdit(QWidget *parent = 0);
    virtual ~ValidatedLineEdit();

    void setValidator(PredefinedValidator validator);
    void setValidator(const QString &regexp, const QString &errorMessage);

    QValidator::State validationState();

protected Q_SLOTS:
    void onTextChanged(const QString &text);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    void updateIcon();
    void validate();

private:
    class Private;
    Private *d;
};

#endif // VALIDATEDLINEEDIT_H
