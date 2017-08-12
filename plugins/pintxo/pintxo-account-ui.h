/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2013 Anant Kamath <kamathanant@gmail.com>
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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_PINTXO_ACCOUNT_UI_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_PINTXO_ACCOUNT_UI_H

#include <KCMTelepathyAccounts/AbstractAccountUi>

class PintxoAccountUi : public AbstractAccountUi
{
    Q_OBJECT

public:
    explicit PintxoAccountUi(QObject *parent = 0);
    virtual ~PintxoAccountUi();

    virtual AbstractAccountParametersWidget
              *mainOptionsWidget(ParameterEditModel *model,
                                 QWidget *parent = 0) const;
private:
    Q_DISABLE_COPY(PintxoAccountUi)
};


#endif // header guard
