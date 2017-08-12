/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#ifndef TELEPATHY_ACCOUNTS_KCM_PARAMETER_EDIT_WIDGET_H
#define TELEPATHY_ACCOUNTS_KCM_PARAMETER_EDIT_WIDGET_H

#include <KCMTelepathyAccounts/abstract-account-parameters-widget.h>

#include "kcm_telepathy_accounts_export.h"

#include <TelepathyQt/ConnectionManager>

class KCM_TELEPATHY_ACCOUNTS_EXPORT ParameterEditWidget : public AbstractAccountParametersWidget
{
    Q_OBJECT

public:
    explicit ParameterEditWidget(ParameterEditModel *parameterModel,
                                 QWidget *parent = 0);
    ~ParameterEditWidget();

    virtual QString defaultDisplayName() const;
private Q_SLOTS:
    void onDelegateDataChanged(const QModelIndex &index, const QVariant &value, int role);

private:
    Q_DISABLE_COPY(ParameterEditWidget)

    class Private;
    Private * const d;
};


#endif  // Header guard

