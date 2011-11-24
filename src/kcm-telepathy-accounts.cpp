/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "kcm-telepathy-accounts.h"

#include "ui_main-widget.h"

#include "accounts-list-model.h"
#include "add-account-assistant.h"
#include "edit-account-dialog.h"
#include "accounts-list-delegate.h"

#include <KTelepathy/wallet-interface.h>

#include <QLabel>
#include <QSortFilterProxyModel>

#include <KGenericFactory>
#include <KIcon>
#include <KLocale>
#include <KMessageBox>
#include <KAboutData>
#include <KMessageWidget>
#include <KPixmapSequenceOverlayPainter>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountFactory>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/Types>
#include <TelepathyQt4/ConnectionManager>

#include "salut-enabler.h"
#include <KPixmapSequence>

K_PLUGIN_FACTORY(KCMTelepathyAccountsFactory, registerPlugin<KCMTelepathyAccounts>();)
K_EXPORT_PLUGIN(KCMTelepathyAccountsFactory("telepathy_accounts", "telepathy-accounts-kcm"))


KCMTelepathyAccounts::KCMTelepathyAccounts(QWidget *parent, const QVariantList& args)
 : KCModule(KCMTelepathyAccountsFactory::componentData(), parent, args),
   m_accountsListModel(0)
{
    kDebug();

    //set up component data.
    KAboutData *aboutData = new KAboutData(I18N_NOOP("telepathy_accounts"), 0, ki18n("Instant Messaging and VOIP Accounts"), "0.2.60", KLocalizedString(), KAboutData::License_GPL);

    aboutData->addAuthor(ki18n("George Goldberg"), ki18n("Developer"),"grundleborg@googlemail.com");
    aboutData->addAuthor(ki18n("David Edmundson"), ki18n("Developer"), "david@davidedmundson.co.uk");
    aboutData->addAuthor(ki18n("Dominik Schmidt"), ki18n("Developer"), "kde@dominik-schmidt.de");
    aboutData->addAuthor(ki18n("Thomas Richard"), ki18n("Developer"), "thomas.richard@proan.be");
    aboutData->addAuthor(ki18n("Florian Reinhard"), ki18n("Developer"), "florian.reinhard@googlemail.com");
    aboutData->addAuthor(ki18n("Daniele E. Domenichelli"), ki18n("Developer"), "daniele.domenichelli@gmail.com");
    aboutData->addAuthor(ki18n("Martin Klapetek"), ki18n("Developer"), "martin.klapetek@gmail.com");

    setAboutData(aboutData);

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
    m_ui->salutListView->setHidden(true);
    m_ui->salutEnableFrame->setHidden(true);
    m_ui->salutEnableCheckbox->setIcon(KIcon("im-local-xmpp"));
    m_ui->salutEnableCheckbox->setIconSize(QSize(32, 32));
    m_accountsListModel = new AccountsListModel(this);

    // On the kcm_telepathy_account we filter using "local-xmpp" and not using
    // "salut" because in this way we can see on top also local-xmpp accounts
    // configured using haze
    m_salutFilterModel = new QSortFilterProxyModel(this);
    m_salutFilterModel->setSourceModel(m_accountsListModel);
    m_salutFilterModel->setFilterRole(AccountsListModel::ConnectionProtocolNameRole);
    m_salutFilterModel->setFilterFixedString(QLatin1String("local-xmpp"));
    m_ui->salutListView->setModel(m_salutFilterModel);

    m_accountsFilterModel = new QSortFilterProxyModel(this);
    m_accountsFilterModel->setSourceModel(m_accountsListModel);
    m_accountsFilterModel->setFilterRole(AccountsListModel::ConnectionProtocolNameRole);
    m_accountsFilterModel->setFilterRegExp(QLatin1String("^((?!local-xmpp).)*$"));
    m_accountsFilterModel->setSortRole(Qt::DisplayRole);
    m_accountsFilterModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_accountsFilterModel->setDynamicSortFilter(true);
    m_accountsFilterModel->sort(0);
    m_ui->accountsListView->setModel(m_accountsFilterModel);

    m_ui->addAccountButton->setIcon(KIcon("list-add"));
    m_ui->editAccountButton->setIcon(KIcon("configure"));
    m_ui->removeAccountButton->setIcon(KIcon("edit-delete"));

    m_salutBusyWheel = new KPixmapSequenceOverlayPainter(this);
    m_salutBusyWheel->setSequence(KPixmapSequence("process-working", 22));
    m_salutBusyWheel->setWidget(m_ui->salutWidget);
    m_salutBusyWheel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    AccountsListDelegate* accountsDelegate = new AccountsListDelegate(m_ui->accountsListView, this);
    m_ui->accountsListView->setItemDelegate(accountsDelegate);

    AccountsListDelegate* salutDelegate = new AccountsListDelegate(m_ui->salutListView, this);
    m_ui->salutListView->setItemDelegate(salutDelegate);

    int height = salutDelegate->sizeHint(QStyleOptionViewItem(), m_salutFilterModel->index(0,0)).height() + 4*2;
    m_ui->salutListView->setMinimumHeight(height);
    m_ui->salutEnableFrame->setMinimumHeight(height);

    connect(accountsDelegate,
            SIGNAL(itemChecked(QModelIndex, bool)),
            SLOT(onAccountEnabledChanged(QModelIndex, bool)));
    connect(salutDelegate,
            SIGNAL(itemChecked(QModelIndex, bool)),
            SLOT(onAccountEnabledChanged(QModelIndex, bool)));
    connect(m_ui->addAccountButton,
            SIGNAL(clicked()),
            SLOT(onAddAccountClicked()));
    connect(m_ui->editAccountButton,
            SIGNAL(clicked()),
            SLOT(onEditAccountClicked()));
    connect(m_ui->accountsListView,
            SIGNAL(doubleClicked(QModelIndex)),
            SLOT(onEditAccountClicked()));
    connect(m_ui->salutListView,
            SIGNAL(doubleClicked(QModelIndex)),
            SLOT(onEditAccountClicked()));
    connect(m_ui->removeAccountButton,
            SIGNAL(clicked()),
            SLOT(onRemoveAccountClicked()));
    connect(m_ui->accountsListView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(onSelectedItemChanged(QModelIndex, QModelIndex)));
    connect(m_ui->salutListView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(onSelectedItemChanged(QModelIndex, QModelIndex)));
    connect(m_accountsListModel,
            SIGNAL(rowsInserted(QModelIndex, int, int)),
            SLOT(onModelDataChanged()));
    connect(m_accountsListModel,
            SIGNAL(rowsRemoved(QModelIndex, int, int)),
            SLOT(onModelDataChanged()));
    connect(m_ui->salutEnableCheckbox,
            SIGNAL(toggled(bool)),
            SLOT(onSalutEnableButtonToggled(bool)));

    // Check if salut is enabled
    Tp::ConnectionManagerPtr cm = Tp::ConnectionManager::create(QLatin1String("salut"));
    Tp::PendingReady *op = cm->becomeReady(Tp::Features() << Tp::ConnectionManager::FeatureCore);
    connect(op,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onSalutConnectionManagerReady(Tp::PendingOperation*)));
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

void KCMTelepathyAccounts::onAccountEnabledChanged(const QModelIndex &index, bool enabled)
{
    QVariant value;
    if (enabled) {
        value = QVariant(Qt::Checked);
    }
    else {
        value = QVariant(Qt::Unchecked);
    }
    m_accountsListModel->setData(index, value, Qt::CheckStateRole);

    if (enabled) {
        // connect the account
        AccountItem *item = m_accountsListModel->itemForIndex(index);
        if (item) {
            item->account()->setRequestedPresence(Tp::Presence::available(QString("Online")));
        }
    }
}

void KCMTelepathyAccounts::onAccountManagerReady(Tp::PendingOperation *op)
{
    kDebug();

    // Check the pending operation completed successfully.
    if (op->isError()) {
        kDebug() << "becomeReady() failed:" << op->errorName() << op->errorMessage();
        new ErrorOverlay(this, op->errorMessage(), this);
        return;
    }

    // Add all the accounts to the Accounts Model.
    QList<Tp::AccountPtr> accounts = m_accountManager->allAccounts();
    foreach (Tp::AccountPtr account, accounts) {
        m_accountsListModel->addAccount(account);
    }

    onModelDataChanged();

    connect(m_accountManager.data(),
            SIGNAL(newAccount (Tp::AccountPtr)),
            SLOT(onAccountCreated(Tp::AccountPtr)));
}

void KCMTelepathyAccounts::onAccountCreated(const Tp::AccountPtr &account)
{
    m_accountsListModel->addAccount(account);
}

void KCMTelepathyAccounts::onSelectedItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (!current.isValid()) {
        m_ui->removeAccountButton->setEnabled(false);
        m_ui->editAccountButton->setEnabled(false);
        return;
    }

    m_currentModel = qobject_cast<const QSortFilterProxyModel*>(current.model());

    if (m_currentModel == m_salutFilterModel) {
        m_currentListView = m_ui->salutListView;
        m_ui->accountsListView->clearSelection();
        m_ui->accountsListView->setCurrentIndex(QModelIndex());
    } else {
        m_currentListView = m_ui->accountsListView;
        m_ui->salutListView->clearSelection();
        m_ui->salutListView->setCurrentIndex(QModelIndex());
    }

    bool isAccount = (m_currentListView->currentIndex().isValid());
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

    QModelIndex index = m_currentListView->currentIndex();
    AccountItem *item = m_accountsListModel->itemForIndex(m_currentModel->mapToSource(index));

    if (!item)
        return;

    // Item is OK. Edit the item.
    EditAccountDialog dialog(item, this);
    dialog.exec();
}

void KCMTelepathyAccounts::onRemoveAccountClicked()
{
    kDebug();
    QModelIndex index = m_currentListView->currentIndex();

     if ( KMessageBox::warningContinueCancel(this, i18n("Are you sure you want to remove the account \"%1\"?", m_currentModel->data(index, Qt::DisplayRole).toString()),
                                        i18n("Remove Account"), KGuiItem(i18n("Remove Account"), "edit-delete"), KStandardGuiItem::cancel(),
                                        QString(), KMessageBox::Notify | KMessageBox::Dangerous) == KMessageBox::Continue)
    {
        AccountItem *item = m_accountsListModel->itemForIndex(m_currentModel->mapToSource(index));
        KTp::WalletInterface wallet(this->effectiveWinId());
        wallet.removeAccount(item->account());

        m_accountsListModel->removeAccount(m_currentModel->mapToSource(index));
    }
}

void KCMTelepathyAccounts::onModelDataChanged()
{
    bool salutEnabled = m_salutFilterModel->rowCount() == 0;
    m_ui->salutListView->setHidden(salutEnabled);
    m_ui->salutEnableFrame->setHidden(!salutEnabled);
}

void KCMTelepathyAccounts::onSalutEnableButtonToggled(bool checked)
{
    if (checked) {
        if (m_salutEnabler.isNull()) {
            m_salutEnabler = new SalutEnabler(m_accountManager, this);
        }

        connect(m_salutEnabler.data(), SIGNAL(userInfoReady()),
                this, SLOT(onSalutInfoReady()));

        connect(m_salutEnabler.data(), SIGNAL(cancelled()),
                this, SLOT(onSalutSetupDone()));

        connect(m_salutEnabler.data(), SIGNAL(done()),
                this, SLOT(onSalutSetupDone()));

        //FIXME there is no slot to connect feedbackMessage to
//         connect(m_salutEnabler, SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)),
//                 this, SLOT());

        m_ui->accountsListView->clearSelection();
        m_ui->accountsListView->setCurrentIndex(QModelIndex());
        m_ui->salutListView->clearSelection();
        m_ui->salutListView->setCurrentIndex(QModelIndex());
    }
}

void KCMTelepathyAccounts::onSalutConnectionManagerReady(Tp::PendingOperation* op)
{
    bool error = false;
    if(op->isError()) {
        kWarning() << "Creating salut ConnectionManager failed:" << op->errorName() << op->errorMessage();
        error = true;
    } else {
        Tp::ConnectionManagerPtr cm = Tp::ConnectionManagerPtr::qObjectCast(qobject_cast<Tp::PendingReady*>(op)->proxy());

        if(!cm->isValid()) {
            kWarning() << "Invalid salut ConnectionManager";
            error = true;
        } else if(!cm->supportedProtocols().contains(QLatin1String("local-xmpp"))) {
            kWarning() << "salut ConnectionManager doesn't support local-xmpp... this is weird";
            error = true;
        }
    }

    // Salut is not installed or has some problem
    if(error) {
        m_ui->salutEnableFrame->setDisabled(true);
        m_ui->salutEnableStatusLabel->setText(i18n("Install telepathy-salut to enable"));
    }
}

void KCMTelepathyAccounts::onSalutInfoReady()
{
    qobject_cast<QVBoxLayout*>(m_ui->salutEnableFrame->layout())->insertWidget(0, m_salutEnabler.data()->frameWidget(m_ui->salutEnableFrame));
    m_salutBusyWheel->start();
    m_ui->salutWidget->setDisabled(true);
}

void KCMTelepathyAccounts::onSalutSetupDone()
{
    m_salutEnabler.data()->deleteLater();

    m_salutBusyWheel->stop();
    m_ui->salutEnableCheckbox->setChecked(false);
    m_ui->salutWidget->setEnabled(true);
}

/////

ErrorOverlay::ErrorOverlay(QWidget *baseWidget, const QString &details, QWidget *parent) :
    QWidget(parent ? parent : baseWidget->window()),
    m_BaseWidget(baseWidget)
{
    Q_UNUSED(details)

    // Build the UI
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(10);

    QLabel *pixmap = new QLabel();
    pixmap->setPixmap(KIcon("dialog-error").pixmap(64));

    QLabel *message = new QLabel(i18n("Something went terribly wrong and the IM system could not be initialized.\n"
                                      "It is likely your system is missing Telepathy Mission Control package.\n"
                                      "Please install it and restart this module."));

    pixmap->setAlignment(Qt::AlignHCenter);
    message->setAlignment(Qt::AlignHCenter);

    layout->addStretch();
    layout->addWidget(pixmap);
    layout->addWidget(message);
    layout->addStretch();

    setLayout(layout);

    // Draw the transparent overlay background
    QPalette p = palette();
    p.setColor(backgroundRole(), QColor(0, 0, 0, 128));
    p.setColor(foregroundRole(), Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    m_BaseWidget->installEventFilter(this);

    reposition();
}

ErrorOverlay::~ErrorOverlay()
{
}

void ErrorOverlay::reposition()
{
    if (!m_BaseWidget) {
        return;
    }

    // reparent to the current top level widget of the base widget if needed
    // needed eg. in dock widgets
    if (parentWidget() != m_BaseWidget->window()) {
        setParent(m_BaseWidget->window());
    }

    // follow base widget visibility
    // needed eg. in tab widgets
    if (!m_BaseWidget->isVisible()) {
        hide();
        return;
    }

    show();

    // follow position changes
    const QPoint topLevelPos = m_BaseWidget->mapTo(window(), QPoint(0, 0));
    const QPoint parentPos = parentWidget()->mapFrom(window(), topLevelPos);
    move(parentPos);

    // follow size changes
    // TODO: hide/scale icon if we don't have enough space
    resize(m_BaseWidget->size());
}

bool ErrorOverlay::eventFilter(QObject * object, QEvent * event)
{
    if (object == m_BaseWidget &&
        (event->type() == QEvent::Move || event->type() == QEvent::Resize ||
        event->type() == QEvent::Show || event->type() == QEvent::Hide ||
        event->type() == QEvent::ParentChange)) {
        reposition();
    }
    return QWidget::eventFilter(object, event);
}

#include "kcm-telepathy-accounts.moc"
