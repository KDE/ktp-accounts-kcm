/*
 * This file is part of telepathy-accounts-kcm and contains
 * code from Bluedevil
 *
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@kde.org>
 * Copyright (C) 2010 UFO Coders <info@ufocoders.com>
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

#include "feedback-widget.h"

#include <KTitleWidget>
#include <KDebug>
#include <KLocale>
#include <KIcon>
#include <KPushButton>
#include <KColorScheme>

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtCore/QTimer>

FeedbackWidget::FeedbackWidget(QWidget *parent)
    : KTitleWidget(parent)
{
    // Hide by default because we only want to show ourselfs when an error came up
    this->hide();

    m_type = KTitleWidget::PlainMessage;
}

FeedbackWidget::~FeedbackWidget()
{
}

void FeedbackWidget::setMessage(const QString &text, const QString &comment, KTitleWidget::MessageType type)
{
    setText(text, type);
    setComment(comment, type);
}

void FeedbackWidget::setText(const QString &text, KTitleWidget::MessageType type)
{
    m_type = type;
    KTitleWidget::setText(text, type);
}

void FeedbackWidget::setComment(const QString &comment, KTitleWidget::MessageType type)
{
    m_type = type;
    KTitleWidget::setComment(comment, type);
}

void FeedbackWidget::paintEvent(QPaintEvent *event)
{
    const QRect r = rect();

    const KColorScheme colorScheme(QPalette::Active, KColorScheme::Window);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(0, 0, r.width(), r.height(), 10, 10);

    QBrush brush;

    switch(m_type) {
        case KTitleWidget::PlainMessage:
            brush = colorScheme.background(KColorScheme::NormalBackground);
            break;
        case KTitleWidget::ErrorMessage:
            brush = colorScheme.background(KColorScheme::NegativeBackground);
            break;
        case KTitleWidget::WarningMessage:
            brush = colorScheme.background(KColorScheme::NeutralBackground);
            break;
        case KTitleWidget::InfoMessage:
            brush = QBrush(QColor(156,213,219));
            break;
    }

    p.fillPath(path, brush);

    KTitleWidget::paintEvent(event);
}

#include "feedback-widget.moc"
