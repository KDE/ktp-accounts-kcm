/*
 * This file is part of ktp-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson. <kde@davidedmundson.co.uk>
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

#ifndef ACCOUNT_IDENTITY_DIALOG_H
#define ACCOUNT_IDENTITY_DIALOG_H

#include <KDialog>

#include <TelepathyQt/Types>

namespace Tp {
class PendingOperation;
}

namespace Ui {
    class AccountIdentityDialog;
}

/** Widget that configures user's identity (nickname/avatar etc) for an account*/

class AccountIdentityDialog : public KDialog
{
    Q_OBJECT

public:
    explicit AccountIdentityDialog(const Tp::AccountPtr &account, QWidget *parent = 0);
    ~AccountIdentityDialog();

public Q_SLOTS:
    /** Updates the account to user selection*/
    void apply();

private Q_SLOTS:
    void onNicknameChanged(const QString &nickname);
    void onAvatarChanged(const Tp::Avatar &avatar);

private:
    Tp::AccountPtr m_account;
    Ui::AccountIdentityDialog *ui;
};

#endif // ACCOUNT_IDENTITY_DIALOG_H
