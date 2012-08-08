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

#include "account-item.h"

#include "accounts-list-model.h"
#include "edit-account-dialog.h"

#include <KTp/error-dictionary.h>
#include <KTp/wallet-utils.h>

#include <KApplication>
#include <KDebug>
#include <KIcon>
#include <KLocalizedString>
#include <KPixmapSequence>

#include <QtCore/QTimer>
#include <QtGui/QPainter>

#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/PendingComposite>


AccountItem::AccountItem(const Tp::AccountPtr &account, AccountsListModel *parent)
 : QObject(parent),
   m_account(account),
   m_icon(new KIcon())
{
    //connect AccountPtr signals to AccountItem signals
    connect(m_account.data(),
            SIGNAL(stateChanged(bool)),
            SIGNAL(updated()));
    connect(m_account.data(),
            SIGNAL(displayNameChanged(QString)),
            SIGNAL(updated()));
    connect(m_account.data(),
            SIGNAL(connectionStatusChanged(Tp::ConnectionStatus)),
            SIGNAL(updated()));
    connect(m_account.data(),
            SIGNAL(iconNameChanged(QString)),
            SLOT(generateIcon()));
    connect(m_account.data(),
            SIGNAL(removed()),
            SIGNAL(removed()));
    connect(m_account.data(),
            SIGNAL(stateChanged(bool)),
            SLOT(generateIcon()));

    generateIcon();
}

AccountItem::~AccountItem()
{
    delete m_icon;
}

Tp::AccountPtr AccountItem::account() const
{
    return m_account;
}

Tp::PendingOperation* AccountItem::remove()
{
    QList<Tp::PendingOperation*> ops;
    ops.append(KTp::WalletUtils::removeAccountPassword(m_account));
    ops.append(m_account->remove());
    return new Tp::PendingComposite(ops, m_account);
}

const KIcon& AccountItem::icon() const
{
    Q_ASSERT(m_icon != 0);

    return *m_icon;
}

const QString AccountItem::connectionStateString() const
{
    if (m_account->isEnabled()) {
        switch (m_account->connectionStatus()) {
        case Tp::ConnectionStatusConnected:
            return i18n("Online");
        case Tp::ConnectionStatusConnecting:
            return i18nc("This is a connection state", "Connecting");
        case Tp::ConnectionStatusDisconnected:
            return i18nc("This is a connection state", "Disconnected");
        default:
            return i18nc("This is an unknown connection state", "Unknown");
        }
    } else {
        return i18nc("This is a disabled account", "Disabled");
    }
}

const KIcon AccountItem::connectionStateIcon() const
{
    if (m_account->isEnabled()) {
        switch (m_account->connectionStatus()) {
        case Tp::ConnectionStatusConnected:
            return KIcon(QLatin1String("user-online"));
        case Tp::ConnectionStatusConnecting:
            //imho this is not really worth animating, but feel free to play around..
            return KIcon(KPixmapSequence(QLatin1String("process-working"), 22).frameAt(0));
        case Tp::ConnectionStatusDisconnected:
            return KIcon(QLatin1String("user-offline"));
        default:
            return KIcon(QLatin1String("user-offline"));
        }
    } else {
        return KIcon();
    }
}

const QString AccountItem::connectionStatusReason() const
{
    if (!m_account->isEnabled()) {
        return i18n("Click checkbox to enable");
    }
    else if (m_account->connectionStatusReason() == Tp::ConnectionStatusReasonRequested) {
        return QString();
    }
    else {
        return KTp::ErrorDictionary::displayShortErrorMessage(m_account->connectionError());
    }
}

const QString AccountItem::connectionProtocolName() const
{
    return m_account->protocolName();
}

void AccountItem::generateIcon()
{
    QString iconPath = account()->iconName();

    //if the icon has not been set, we use the protocol icon
    if (iconPath.isEmpty()) {
        iconPath = QString::fromLatin1("im-%1").arg(account()->protocolName());
    }

    delete m_icon;
    if (m_account->isEnabled()) {
        m_icon = new KIcon(iconPath);
    } else {
        m_icon = new KIcon(KIconLoader::global()->loadIcon(iconPath, KIconLoader::Desktop, 32, KIconLoader::DisabledState));
    }

    if (!account()->isValid()) {
        //we paint a warning symbol in the right-bottom corner
        QPixmap pixmap = m_icon->pixmap(32, 32);
        QPainter painter(&pixmap);
        KIcon(QLatin1String("dialog-error")).paint(&painter, 15, 15, 16, 16);

        delete m_icon;
        m_icon = new KIcon(pixmap);
    }

    Q_EMIT(updated());
}

void AccountItem::onTitleForCustomPages(QString mandatoryPage, QList<QString> optionalPage)
{
    Q_EMIT setTitleForCustomPages(mandatoryPage, optionalPage);
}

Tp::ConnectionStatus AccountItem::connectionStatus() const
{
    return m_account->connectionStatus();
}

#include "account-item.moc"
