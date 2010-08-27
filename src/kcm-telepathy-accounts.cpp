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
#include "add-account-assistant.h"

#include <KGenericFactory>
#include <KIcon>
#include <KLocale>
#include <KMessageBox>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/Types>


K_PLUGIN_FACTORY(KCMTelepathyAccountsFactory, registerPlugin<KCMTelepathyAccounts>();)
K_EXPORT_PLUGIN(KCMTelepathyAccountsFactory("telepathy_accounts", "kcm_telepathy_accounts"))


KCMTelepathyAccounts::KCMTelepathyAccounts(QWidget *parent, const QVariantList& args)
 : KCModule(KCMTelepathyAccountsFactory::componentData(), parent, args),
   m_accountsListModel(0),
   m_addAccountAssistant(0)
{
    kDebug();

    // The first thing we must do is register Telepathy DBus Types.
    Tp::registerTypes();

    // Start setting up the Telepathy AccountManager.
    m_accountManager = Tp::AccountManager::create();

    connect(m_accountManager->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountManagerReady(Tp::PendingOperation*)));

    // Set up the UI stuff.
    setupUi(this);

    m_accountsListModel = new AccountsListModel(this);
    m_accountsListView->setModel(m_accountsListModel);

    m_addAccountButton->setIcon(KIcon("list-add"));
    m_editAccountButton->setIcon(KIcon("configure"));
    m_removeAccountButton->setIcon(KIcon("edit-delete"));

    // Connect to useful signals from the UI elements.
    connect(m_addAccountButton,
            SIGNAL(clicked()),
            SLOT(onAddAccountClicked()));
    connect(m_editAccountButton,
            SIGNAL(clicked()),
            SLOT(onEditAccountClicked()));
    connect(m_removeAccountButton,
            SIGNAL(clicked()),
            SLOT(onRemoveAccountClicked()));
    connect(m_accountsListView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(onSelectedItemChanged()));
      connect(this, SIGNAL(setTitleForCustomPages(QString, QList<QString>)),
                  m_accountsListModel, SLOT(onTitleForCustomPages(QString, QList<QString>)));

}

KCMTelepathyAccounts::~KCMTelepathyAccounts()
{
    kDebug();

    // TODO: Implement me!
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
            SIGNAL(accountCreated(const QString &)),
            SLOT(onAccountCreated(const QString &)));
}

void KCMTelepathyAccounts::onAccountCreated(const QString &path)
{
    Tp::AccountPtr account = m_accountManager->accountForPath(path);
    m_accountsListModel->addAccount(account);
}

void KCMTelepathyAccounts::onSelectedItemChanged()
{
    bool isAccount = m_accountsListView->currentIndex().isValid();
    m_removeAccountButton->setEnabled(isAccount);
    m_editAccountButton->setEnabled(isAccount);
}

void KCMTelepathyAccounts::onAddAccountClicked()
{
    kDebug();

    // Wizard only works if the AccountManager is ready.
    if (!m_accountManager->isReady()) {
        return;
    }

    // Ensure that there is not already an instance of the AddAccountAssistant before we create one.";
    if (!m_addAccountAssistant) {

        // Create an AddAccountAssistant instance
        m_addAccountAssistant = new AddAccountAssistant(m_accountManager, this);

        // Connect to its completion signals...
        connect(m_addAccountAssistant, SIGNAL(cancelled()),
                this, SLOT(onAddAccountAssistantClosed()));
        connect(m_addAccountAssistant, SIGNAL(accepted()),
                this, SLOT(onAddAccountAssistantClosed()));
		connect(m_addAccountAssistant, SIGNAL(protocolSelected(QString, QString)),
				this, SLOT(onProtocolSelected(QString, QString)));
		connect(this, SIGNAL(setTitleForCustomPages(QString, QList<QString>)),
				m_addAccountAssistant, SLOT(onTitleForCustomPages(QString, QList<QString>)));

        // ...and finally show it.
        m_addAccountAssistant->show();

        return;
    }

    kWarning() << "Cannot create a new AddAccountAssistant. One already exists.";
}

void KCMTelepathyAccounts::onEditAccountClicked()
{
    kDebug();

    // Editing accounts is only possible if the Account Manager is ready.
    if (!m_accountManager->isReady()) {
        return;
    }

    QModelIndex index = m_accountsListView->currentIndex();

    // A valid account must be selected in the list to allow editing
    if (!index.isValid()) {
        return;
    }

    // Item is OK. Edit the item.
	m_accountsListModel->disconnect();
	connect(m_accountsListModel, SIGNAL(protocolSelected(QString, QString)),
			this, SLOT(onProtocolSelected(QString, QString)));
      
    m_accountsListModel->editAccount(index);
}

void KCMTelepathyAccounts::onRemoveAccountClicked()
{
    kDebug();
    QModelIndex index = m_accountsListView->currentIndex();

     if ( KMessageBox::warningContinueCancel( this, i18n( "Are you sure you want to remove the account \"%1\"?", m_accountsListModel->data(index, Qt::DisplayRole).toString()),
                                        i18n( "Remove Account" ), KGuiItem(i18n( "Remove Account" ), "edit-delete"), KStandardGuiItem::cancel(),
                                        QString(), KMessageBox::Notify | KMessageBox::Dangerous ) == KMessageBox::Continue )
    {
        m_accountsListModel->removeAccount(index);
    }
}

void KCMTelepathyAccounts::onAddAccountAssistantClosed()
{
    kDebug();

    // Add account assistant has been cancelled. Delete it.
    m_addAccountAssistant->deleteLater();
    m_addAccountAssistant = 0;
}

void KCMTelepathyAccounts::onProtocolSelected(QString protocol, QString localizedName)
{
	kDebug() << protocol << localizedName;

	QString mandatoryPage;
	QList<QString> optionalPage;

	if(protocol == "jabber")
	{
		mandatoryPage = i18n("Basic setup");
		optionalPage.push_back(i18n("Account preferences"));
		optionalPage.push_back(i18n("Connection settings"));
		optionalPage.push_back(i18n("Advanced options"));
	}

	emit setTitleForCustomPages(mandatoryPage, optionalPage);
}

#include "kcm-telepathy-accounts.moc"

