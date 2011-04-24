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

#include <KTitleWidget>

class QLabel;
class QHBoxLayout;
class QTimer;

class FeedbackWidget : public KTitleWidget
{
Q_OBJECT
public:
    FeedbackWidget(QWidget *parent = 0);
    virtual ~FeedbackWidget();

public slots:
    void setMessage(const QString &text, const QString &comment, KTitleWidget::MessageType type);
    void setText(const QString &text, KTitleWidget::MessageType type);
    void setComment(const QString &comment, KTitleWidget::MessageType type=KTitleWidget::PlainMessage);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    KTitleWidget::MessageType m_type;
};

#endif
