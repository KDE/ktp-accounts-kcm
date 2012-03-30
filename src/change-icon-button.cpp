/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "change-icon-button.h"

#include <KDE/KDebug>

#include <TelepathyQt/Account>
#include <TelepathyQt/PendingOperation>

ChangeIconButton::ChangeIconButton(QWidget* parent) :
    KIconButton(parent)
{
    setIconType(KIconLoader::NoGroup, KIconLoader::Action);
    setIconSize(KIconLoader::SizeLarge);
    setButtonIconSize(KIconLoader::SizeMedium);
    setFixedSize(KIconLoader::SizeMedium + 10, KIconLoader::SizeMedium + 10);

    connect(this, SIGNAL(iconChanged(QString)), SLOT(onIconChanged(QString)));
}

ChangeIconButton::~ChangeIconButton()
{
}

void ChangeIconButton::setAccount(const Tp::AccountPtr &account)
{
    m_account = account;
}

Tp::AccountPtr ChangeIconButton::account() const
{
    return m_account;
}

void ChangeIconButton::onIconChanged(const QString& icon)
{
    kDebug();
    if (!m_account.isNull() && m_account->isValid()) {
        kDebug() << "Setting icon" << icon << "for account" << account()->uniqueIdentifier();
        Tp::PendingOperation *op = m_account->setIconName(icon);
        connect(op, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onFinished(Tp::PendingOperation*)));
    }
}

void ChangeIconButton::onFinished(Tp::PendingOperation* op)
{
    if (op->isError()) {
        kWarning() << "Cannot set icon" << op->errorName() << op->errorMessage();
    }
}
