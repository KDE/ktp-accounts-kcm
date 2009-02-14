/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

#ifndef TELEPATHY_ACCOUNTS_KCM_ACCOUNT_ITEM_H
#define TELEPATHY_ACCOUNTS_KCM_ACCOUNT_ITEM_H

#include <QtCore/QObject>

class AccountsListModel;

namespace Telepathy {
    namespace Client {
        class Account;
        class PendingOperation;
    }
}

class AccountItem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AccountItem);

public:
    explicit AccountItem(Telepathy::Client::Account *account, AccountsListModel *parent = 0);
    virtual ~AccountItem();
    Telepathy::Client::Account* account() const;

private Q_SLOTS:
    void onBecomeReadyFinished(Telepathy::Client::PendingOperation *op);

Q_SIGNALS:
    void ready();

private:
    Telepathy::Client::Account *m_account;

};

#endif // header guard

