/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#ifndef KCM_TELEPATHY_SALUT_ENABLE_ACCOUNT_DIALOG_H
#define KCM_TELEPATHY_SALUT_ENABLE_ACCOUNT_DIALOG_H

#include <KDialog>
#include <KMessageWidget>

#include <TelepathyQt/Types>

class SalutDetailsDialog : public KDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(SalutDetailsDialog);

public:
    explicit SalutDetailsDialog(const Tp::ProfileManagerPtr profileManager, const Tp::ConnectionManagerPtr connectionManager, QWidget *parent = 0);
    virtual ~SalutDetailsDialog();

Q_SIGNALS:
    void feedbackMessage(const QString &text, const QString &comment, KMessageWidget::MessageType);
    void dialogAccepted(const QString &displayName, const QVariantMap &values);

private:
    void accept();

    class Private;
    Private * const d;
};


#endif // KCM_TELEPATHY_SALUT_ENABLE_ACCOUNT_DIALOG_H
