/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2013 Maksim Melnikau <maxposedon@gmail.com>
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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_SIPE_ACCOUNT_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_SIPE_ACCOUNT_H

#include <KCMTelepathyAccounts/AbstractAccountUi>

class SipeAccountUi : public AbstractAccountUi
{
    Q_OBJECT

public:
    explicit SipeAccountUi(QObject *parent = 0);
    virtual ~SipeAccountUi();

    virtual AbstractAccountParametersWidget* mainOptionsWidget(ParameterEditModel *model, QWidget *parent = 0) const;
    virtual bool hasAdvancedOptionsWidget() const;
    virtual AbstractAccountParametersWidget* advancedOptionsWidget(ParameterEditModel *model, QWidget *parent = 0) const;

private:
    Q_DISABLE_COPY(SipeAccountUi)
};

#endif // header guard
