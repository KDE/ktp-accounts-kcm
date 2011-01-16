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

#include "ui_main-widget.h"

#include "accounts-list-model.h"
#include "add-account-assistant.h"
#include "edit-account-dialog.h"

#include <KGenericFactory>
#include <KIcon>
#include <KLocale>
#include <KMessageBox>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountFactory>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/Types>


K_PLUGIN_FACTORY(KCMTelepathyAccountsFactory, registerPlugin<KCMTelepathyAccounts>();)
K_EXPORT_PLUGIN(KCMTelepathyAccountsFactory("telepathy_accounts", "kcm_telepathy_accounts"))


KCMTelepathyAccounts::KCMTelepathyAccounts(QWidget *parent, const QVariantList& args)
 : KCModule(KCMTelepathyAccountsFactory::componentData(), parent, args),
   m_accountsListModel(0)
{
    kDebug();

    // The first thing we must do is register Telepathy DBus Types.
    Tp::registerTypes();

    // Start setting up the Telepathy AccountManager.
    Tp::AccountFactoryPtr  accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                       Tp::Features() << Tp::Account::FeatureCore
                                                                       << Tp::Account::FeatureAvatar
                                                                       << Tp::Account::FeatureProtocolInfo
                                                                       << Tp::Account::FeatureProfile);

    m_accountManager = Tp::AccountManager::create(accountFactory);

    connect(m_accountManager->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountManagerReady(Tp::PendingOperation*)));

    // Set up the UI stuff.
    m_ui = new Ui::MainWidget;
    m_ui->setupUi(this);

    m_accountsListModel = new AccountsListModel(this);
    m_ui->accountsListView->setModel(m_accountsListModel);

    m_ui->addAccountButton->setIcon(KIcon("list-add"));
    m_ui->editAccountButton->setIcon(KIcon("configure"));
    m_ui->removeAccountButton->setIcon(KIcon("edit-delete"));

    // Connect to useful signals from the UI elements.
    connect(m_ui->addAccountButton,
            SIGNAL(clicked()),
            SLOT(onAddAccountClicked()));
    connect(m_ui->editAccountButton,
            SIGNAL(clicked()),
            SLOT(onEditAccountClicked()));
    connect(m_ui->removeAccountButton,
            SIGNAL(clicked()),
            SLOT(onRemoveAccountClicked()));
    connect(m_ui->accountsListView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(onSelectedItemChanged()));
}

KCMTelepathyAccounts::~KCMTelepathyAccounts()
{
    kDebug();

    delete m_ui;
}

void KCMTelepathyAccounts::load()
{
    kDebug();

    // This slot is called whenever the configuration data in this KCM should
    // be reloaded from the store. We will not actually do anything here since
    // all changes that are made in this KCM are, at the moment, saved
    // immediately and cannot be reverted programatically.
    return;
}

void KCMTelepathyAccounts::onAccountManagerReady(Tp::PendingOperation *op)
{
    kDebug();

    // Check the pending operation completed successfully.
    if (op->isError()) {
        kDebug() << "becomeReady() failed:" << op->errorName() << op->errorMessage();
        return;
    }

    // Add all the accounts to the Accounts Model.
    QList<Tp::AccountPtr> accounts = m_accountManager->allAccounts();
    foreach (Tp::AccountPtr account, accounts) {
        m_accountsListModel->addAccount(account);
    }

    connect(m_accountManager.data(),
            SIGNAL(newAccount (Tp::AccountPtr)),
            SLOT(onAccountCreated(Tp::AccountPtr)));
}

void KCMTelepathyAccounts::onAccountCreated(const Tp::AccountPtr &account)
{
    m_accountsListModel->addAccount(account);
}

void KCMTelepathyAccounts::onSelectedItemChanged()
{
    bool isAccount = m_ui->accountsListView->currentIndex().isValid();
    m_ui->removeAccountButton->setEnabled(isAccount);
    m_ui->editAccountButton->setEnabled(isAccount);
}

void KCMTelepathyAccounts::onAddAccountClicked()
{
    kDebug();

    // Wizard only works if the AccountManager is ready.
    if (!m_accountManager->isReady()) {
        return;
    }

    // ...and finally exec it.
    AddAccountAssistant assistant(m_accountManager, this);
    assistant.exec();
}

void KCMTelepathyAccounts::onEditAccountClicked()
{
    kDebug();

    // Editing accounts is only possible if the Account Manager is ready.
    if (!m_accountManager->isReady()) {
        return;
    }

    QModelIndex index = m_ui->accountsListView->currentIndex();
    AccountItem *item = m_accountsListModel->itemForIndex(index);

    if (!item)
        return;

    // Item is OK. Edit the item.
    EditAccountDialog dialog(item, this);
    dialog.exec();
}

void KCMTelepathyAccounts::onRemoveAccountClicked()
{
    kDebug();
    QModelIndex index = m_ui->accountsListView->currentIndex();

     if ( KMessageBox::warningContinueCancel( this, i18n( "Are you sure you want to remove the account \"%1\"?", m_accountsListModel->data(index, Qt::DisplayRole).toString()),
                                        i18n( "Remove Account" ), KGuiItem(i18n( "Remove Account" ), "edit-delete"), KStandardGuiItem::cancel(),
                                        QString(), KMessageBox::Notify | KMessageBox::Dangerous ) == KMessageBox::Continue )
    {
        m_accountsListModel->removeAccount(index);
    }
}

#include "kcm-telepathy-accounts.moc"

