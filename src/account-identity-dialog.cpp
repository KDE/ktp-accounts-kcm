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

#include "account-identity-dialog.h"
#include "ui_account-identity-dialog.h"

#include <TelepathyQt/Account>
#include <TelepathyQt/AvatarData>
#include <TelepathyQt/PendingOperation>

//FIXME possibly need to monitor account connection status and disable if appropriate?

AccountIdentityDialog::AccountIdentityDialog(const Tp::AccountPtr &account, QWidget *parent) :
    KDialog(parent),
    m_account(account),
    ui(new Ui::AccountIdentityDialog)
{
    QWidget *widget = new QWidget(this);
    ui->setupUi(widget);

    Q_ASSERT(! m_account.isNull());

    setMainWidget(widget);

    setWindowTitle(i18n("Edit Account Identity"));
    setButtons(KDialog::Ok | KDialog::Cancel);

    connect(m_account.data(), SIGNAL(nicknameChanged(QString)), SLOT(onNicknameChanged(QString)));
    connect(m_account.data(), SIGNAL(avatarChanged(TpDBus::Avatar)), SLOT(onAvatarChanged(TpDBus::Avatar)));
    onNicknameChanged(account->nickname());
    onAvatarChanged(account->avatar());

    ui->accountId->setText(m_account->displayName());
    ui->accountAvatar->setAccount(m_account);

    connect(this, SIGNAL(okClicked()), SLOT(apply()));
}

AccountIdentityDialog::~AccountIdentityDialog()
{
    delete ui;
}

void AccountIdentityDialog::onNicknameChanged(const QString &nickname)
{
    ui->accountNickname->setText(nickname);
}

void AccountIdentityDialog::onAvatarChanged(const TpDBus::Avatar &avatar)
{
    ui->accountAvatar->setAvatar(avatar);
}

void AccountIdentityDialog::apply()
{
    if (!m_account.isNull()) {
        //not much point watching these, they just return that everything was OK even when it isn't.
        m_account->setAvatar(ui->accountAvatar->avatar());
        m_account->setNickname(ui->accountNickname->text());
    }

    close();
}

#include "account-identity-dialog.moc"
