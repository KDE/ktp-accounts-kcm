/*
 * This file is part of telepathy-accounts-kcm-plugins
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_RAKIA_ACCOUNT_UI_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_RAKIA_ACCOUNT_UI_H

#include <KCMTelepathyAccounts/AbstractAccountUi>

class RakiaAccountUi : public AbstractAccountUi
{
    Q_OBJECT

public:
    explicit RakiaAccountUi(QObject *parent = 0);
    virtual ~RakiaAccountUi();

    virtual AbstractAccountParametersWidget
    *mainOptionsWidget(ParameterEditModel *model,
                       QWidget *parent = 0) const;
    virtual bool hasAdvancedOptionsWidget() const;
    virtual AbstractAccountParametersWidget
    *advancedOptionsWidget(ParameterEditModel *model,
                           QWidget *parent = 0) const;

private:
    Q_DISABLE_COPY(RakiaAccountUi);
};

#endif // header guard
