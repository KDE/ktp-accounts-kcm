/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#ifndef TELEPATHY_ACCOUNTS_KCM_KCM_TELEPATHY_ACCOUNTS_H
#define TELEPATHY_ACCOUNTS_KCM_KCM_TELEPATHY_ACCOUNTS_H

#include <KCModule>

#include <TelepathyQt/AccountManager>

class QFrame;
class KPixmapSequenceOverlayPainter;
class SalutEnabler;
class AccountsListModel;
class AddAccountAssistant;
class QSortFilterProxyModel;
class QListView;

namespace Tp {
    class PendingOperation;
}

namespace Ui {
    class MainWidget;
}

class KCMTelepathyAccounts : public KCModule
{
    Q_OBJECT
    Q_DISABLE_COPY(KCMTelepathyAccounts);

public:
    explicit KCMTelepathyAccounts(QWidget *parent = 0,
                                  const QVariantList& args = QVariantList());
    virtual ~KCMTelepathyAccounts();


public Q_SLOTS:
    virtual void load();

private Q_SLOTS:
    void onAccountManagerReady(Tp::PendingOperation *op);
    void onAccountEnabledChanged(const QModelIndex &index, bool enabled);
    void onSelectedItemChanged(const QModelIndex &current, const QModelIndex &previous);
    void onAddAccountClicked();
    void onEditAccountClicked();
    void onEditIdentityClicked();
    void onRemoveAccountClicked();
    void onModelDataChanged();
    void onSalutEnableButtonToggled(bool checked);
    void onSalutConnectionManagerReady(Tp::PendingOperation*);
    void onSalutInfoReady();

    void onSalutSetupDone();

private:
    Ui::MainWidget *m_ui;

    Tp::AccountManagerPtr m_accountManager;
    AccountsListModel *m_accountsListModel;
    QSortFilterProxyModel *m_salutFilterModel;
    QSortFilterProxyModel *m_accountsFilterModel;
    const QSortFilterProxyModel *m_currentModel;
    const QListView *m_currentListView;

    QWeakPointer<SalutEnabler> m_salutEnabler;
    KPixmapSequenceOverlayPainter *m_salutBusyWheel;
};

// Helper class for drawing error overlays
class ErrorOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit ErrorOverlay(QWidget *baseWidget, const QString &details, QWidget *parent = 0);
    ~ErrorOverlay();

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    void reposition();

private:
    QWidget *m_BaseWidget;
};

#endif // header guard
