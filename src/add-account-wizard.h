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

#ifndef TELEPATHY_ACCOUNTS_KCM_ADD_ACCOUNT_WIZARD_H
#define TELEPATHY_ACCOUNTS_KCM_ADD_ACCOUNT_WIZARD_H

#include <KAssistantDialog>

class AddAccountWizard : public KAssistantDialog
{
    Q_OBJECT

public:
    explicit AddAccountWizard(QWidget *parent = 0);
    ~AddAccountWizard();

protected Q_SLOTS:
    virtual void back();
    virtual void next();
    virtual void accept();
    virtual void reject();

};


#endif  // Header guard

