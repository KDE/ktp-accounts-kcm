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

#include "kcm-telepathy-accounts.h"

#include "accounts-list-model.h"

#include <kcategorizedsortfilterproxymodel.h>
#include <kgenericfactory.h>

#include <TelepathyQt4/Client/Account>
#include <TelepathyQt4/Client/AccountManager>
#include <TelepathyQt4/Client/PendingOperation>
#include <TelepathyQt4/Client/PendingReadyAccount>


K_PLUGIN_FACTORY(KCMTelepathyAccountsFactory, registerPlugin<KCMTelepathyAccounts>();)
K_EXPORT_PLUGIN(KCMTelepathyAccountsFactory("telepathy_accounts", "kcm_telepathy_accounts"))


KCMTelepathyAccounts::KCMTelepathyAccounts(QWidget *parent, const QVariantList& args)
 : KCModule(KCMTelepathyAccountsFactory::componentData(), parent, args),
   m_accountsListProxyModel(0),
   m_accountManager(0),
   m_accountsListModel(0)
{
    // TODO: Implement me!
    setupUi(this);
    startAccountManager();
    m_accountsListModel = new AccountsListModel(this);
}

KCMTelepathyAccounts::~KCMTelepathyAccounts()
{
    // TODO: Implement me!
}

void KCMTelepathyAccounts::load()
{
    // This slot is called whenever the configuration data in this KCM should
    // be reloaded from the store. We will not actually do anything here since
    // all changes that are made in this KCM are, at the moment, saved
    // immediately and cannot be reverted programatically.
    return;
}

void KCMTelepathyAccounts::startAccountManager()
{
    // This slot is called on construction to set up a telepathy accountmanager
    // instance.
    m_accountManager = new Telepathy::Client::AccountManager(this);

    connect(m_accountManager->becomeReady(), SIGNAL(finished(Telepathy::Client::PendingOperation*)),
            this, SLOT(startAccountManagerFinished(Telepathy::Client::PendingOperation*)));
}

void KCMTelepathyAccounts::startAccountManagerFinished(Telepathy::Client::PendingOperation *op)
{
    disconnect(op, SIGNAL(finished(Telepathy::Client::PendingOperation*)),
            this, SLOT(startAccountManagerFinished(Telepathy::Client::PendingOperation*)));

    Q_ASSERT(op->isFinished());
    if(op->isError())
    {
        kDebug() << "An error occurred making the AccountManager ready.";
        return;
    }
    else
    {
        kDebug() << "AccountManager became ready successfully.";
    }

    QList<Telepathy::Client::Account*> accounts = m_accountManager->allAccounts();
    foreach(Telepathy::Client::Account* account, accounts)
    {
        connect(account->becomeReady(), SIGNAL(finished(Telepathy::Client::PendingOperation*)),
                this, SLOT(onAccountReady(Telepathy::Client::PendingOperation*)));
    }
}

void KCMTelepathyAccounts::onAccountReady(Telepathy::Client::PendingOperation *op)
{
    disconnect(op, SIGNAL(finished(Telepathy::Client::PendingOperation*)),
            this, SLOT(onAccountReady(Telepathy::Client::PendingOperation*)));

    Q_ASSERT(op->isFinished());

    Telepathy::Client::PendingReadyAccount *pra = qobject_cast<Telepathy::Client::PendingReadyAccount*>(op);
    Q_ASSERT(0 != pra);

    if(pra->isError())
    {
        kDebug() << "An error occurred in making and Account ready.";
        return;
    }
    else
    {
        kDebug() << "An Account became ready successfully.";
        // TODO: Add the account to the model.
    }
}


#include "kcm-telepathy-accounts.moc"

