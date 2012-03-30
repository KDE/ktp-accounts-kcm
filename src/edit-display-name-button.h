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

#ifndef EDIT_DISPLAY_NAME_BUTTON_H
#define EDIT_DISPLAY_NAME_BUTTON_H

#include <KDE/KPushButton>
#include <TelepathyQt/Types>

namespace Tp { class PendingOperation; }

class EditDisplayNameButton : public KPushButton
{
    Q_OBJECT
    Q_DISABLE_COPY(EditDisplayNameButton)

public:
    explicit EditDisplayNameButton(QWidget *parent = 0);
    virtual ~EditDisplayNameButton();

    void setAccount(const Tp::AccountPtr &account);
    Tp::AccountPtr account() const;

private Q_SLOTS:
    void onClicked();
    void onFinished(Tp::PendingOperation* op);

private:
    Tp::AccountPtr m_account;
};

#endif // EDIT_DISPLAY_NAME_BUTTON_H
