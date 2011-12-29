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

#include "validated-line-edit.h"

#include "parameter-edit-model.h"

#include <KIcon>
#include <KLineEdit>
#include <KLocale>
#include <KIconLoader>

#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QPersistentModelIndex>

class ValidationIconWidget : public QWidget
{
public:
    enum Icon {
        InvalidIcon = 0,
        ValidIcon = 1
    };

    ValidationIconWidget(QWidget *parent);

    void setPixmap(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_pixmap;
};

ValidationIconWidget::ValidationIconWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setCursor(Qt::ArrowCursor);
}

void ValidationIconWidget::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

void ValidationIconWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.drawPixmap(0, 0, width(), height(), m_pixmap);
}

//////////////////////////////////////////////////////////////////////

class ValidatedLineEdit::Private
{
    public:
    Private()
        : validationIcon(0)
    {

    }

    ValidationIconWidget *validationIcon;
    QRegExpValidator validator;
    QValidator::State currentState;
    ValidationIconWidget::Icon currentIcon;
    QString errorMessage;
};

ValidatedLineEdit::ValidatedLineEdit(QWidget *parent)
    : KLineEdit(parent),
      d(new Private())
{
    d->validationIcon = new ValidationIconWidget(this);
    setToolTip(i18n("This field is required"));
    updateIcon();
    setStyleSheet(QLatin1String("QLineEdit { margin-right: 17px;} "));
    validate();

    connect(this,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(onTextChanged(QString)));
}

ValidatedLineEdit::~ValidatedLineEdit()
{
    delete d;
}

void ValidatedLineEdit::setValidator(PredefinedValidator validator)
{
    switch (validator) {
    case NotEmptyValidator:
        setValidator(QLatin1String(".+"),
                     i18n("This field should not be empty"));
        break;
    case EmailValidator:
        setValidator(QLatin1String("[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}"),
                     i18n("This field should contain an email address"));
        break;
    case HostnameValidator:
        setValidator(QLatin1String(".+\\..+"),
                     i18n("This field should contain a hostname"));
        break;
    case IPAddressValidator:
        setValidator(QLatin1String("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"),
                     i18n("This field should contain an IP address"));
        break;
    }
}

void ValidatedLineEdit::setValidator(const QString &regexp, const QString &errorMessage)
{
    QRegExp regexpression(regexp, Qt::CaseInsensitive);
    d->validator.setRegExp(regexpression);
    d->errorMessage = errorMessage;
    validate();
}

QValidator::State ValidatedLineEdit::validationState()
{
    return d->currentState;
}

void ValidatedLineEdit::onTextChanged(const QString &text)
{
    Q_UNUSED(text);
    validate();
}

void ValidatedLineEdit::resizeEvent(QResizeEvent *event)
{
    updateIcon();
    KLineEdit::resizeEvent(event);
}

void ValidatedLineEdit::updateIcon()
{
    d->validationIcon->resize(height()-6, height()-6);
    d->validationIcon->move(width() - d->validationIcon->width(), 3);
    d->validationIcon->update();
}

void ValidatedLineEdit::validate()
{
    int pos;
    QString validatedText(text());

    if (d->validator.validate(validatedText, pos) == QValidator::Acceptable) {
        if (d->currentIcon != ValidationIconWidget::ValidIcon) {
            d->validationIcon->setPixmap(SmallIcon(QLatin1String("dialog-ok-apply"), 0));
            setToolTip(i18n("This field is valid"));
            d->currentIcon = ValidationIconWidget::ValidIcon;
            d->currentState = QValidator::Acceptable;
        }
    } else if (d->currentIcon != ValidationIconWidget::InvalidIcon) {
        d->validationIcon->setPixmap(SmallIcon(QLatin1String("dialog-error"), 0));
        this->setToolTip(d->errorMessage);
        d->currentIcon = ValidationIconWidget::InvalidIcon;
        d->currentState = QValidator::Invalid;
    }
}
