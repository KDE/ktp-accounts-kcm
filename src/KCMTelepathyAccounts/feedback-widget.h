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

#ifndef TELEPATHY_ACCOUNTS_KCM_FEEDBACK_WIDGET_H
#define TELEPATHY_ACCOUNTS_KCM_FEEDBACK_WIDGET_H

#include <KMessageWidget>

class QLabel;
class QHBoxLayout;
class QTimer;

class FeedbackWidget : public KMessageWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(FeedbackWidget)

public:
    FeedbackWidget(QWidget *parent = nullptr);
    ~FeedbackWidget() override;

public Q_SLOTS:
    void setMessage(const QString &text, const QString &comment, KMessageWidget::MessageType type);
    void setText(const QString &text, KMessageWidget::MessageType type);
};

#endif
