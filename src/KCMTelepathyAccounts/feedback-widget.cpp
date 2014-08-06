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

#include <KLocale>
#include <KPushButton>
#include <KColorScheme>

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <QIcon>

FeedbackWidget::FeedbackWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    // Hide by default because we only want to show ourselfs when an error came up
    this->hide();

    this->setMessageType(KMessageWidget::Information);
}

FeedbackWidget::~FeedbackWidget()
{
}

void FeedbackWidget::setMessage(const QString &text, const QString &comment, KMessageWidget::MessageType type)
{
    setText(QString::fromLatin1("<b>%1</b><br />%2").arg(text).arg(comment), type);
}

void FeedbackWidget::setText(const QString &text, KMessageWidget::MessageType type)
{
    this->setMessageType(type);
    KMessageWidget::setText(text);
    this->animatedShow();
}

#include "feedback-widget.moc"
