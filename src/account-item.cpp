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

#include "account-item.h"

#include "accounts-list-model.h"

#include <kdebug.h>

#include <QtCore/QTimer>

#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>

AccountItem::AccountItem(Tp::AccountPtr account, AccountsListModel *parent)
 : QObject(parent),
   m_account(account)
{
    // We should look to see if the "account" instance we are passed is ready
    // yet. If not, we should get it ready now.
    // FIXME: What features should we check are ready?
    if(m_account->isReady())
    {
        QTimer::singleShot(0, this, SIGNAL(ready()));
    }
    else
    {
        // FIXME: What features should we get ready with?
        connect(m_account->becomeReady(), SIGNAL(finished(Tp::PendingOperation*)),
                this, SLOT(onBecomeReadyFinished(Tp::PendingOperation*)), Qt::QueuedConnection);
    }
}

AccountItem::~AccountItem()
{
    // TODO: Implement me...
}

Tp::AccountPtr AccountItem::account() const
{
    return m_account;
}

void AccountItem::onBecomeReadyFinished(Tp::PendingOperation *op)
{

    if(op->isError())
    {
        kDebug() << "An error occurred in making and Account ready.";
        return;
    }

    Q_EMIT ready();
}

#include "account-item.moc"

