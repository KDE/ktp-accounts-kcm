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

#ifndef KCM_TELEPATHY_ACCOUNTS_EDIT_ACCOUNT_DIALOG_H
#define KCM_TELEPATHY_ACCOUNTS_EDIT_ACCOUNT_DIALOG_H

#include <KDialog>

#include <TelepathyQt/Types>
#include <TelepathyQt/PendingOperation>

class EditAccountDialog : public KDialog
{
    Q_OBJECT

public:
    explicit EditAccountDialog(const Tp::AccountPtr &account, QWidget *parent = 0);
    virtual ~EditAccountDialog();

    virtual void setVisible(bool visible);
    virtual void accept();

private Q_SLOTS:
    void onWalletOpened(Tp::PendingOperation *op);
    void onParametersUpdated(Tp::PendingOperation *op);
    void onDisplayNameUpdated(Tp::PendingOperation *op);

private:
    Q_DISABLE_COPY(EditAccountDialog);

    void onFinished();

    class Private;
    Private * const d;
};


#endif // header guard

