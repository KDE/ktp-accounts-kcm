/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Lasse Liehu <lliehu@kolumbus.fi>
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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_IDLE_ACCOUNT_ADVANCED_PARAMETERS_WIDGET_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_IDLE_ACCOUNT_ADVANCED_PARAMETERS_WIDGET_H

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

class AdvancedOptionsWidget : public AbstractAccountParametersWidget
{
    Q_OBJECT

public:
    explicit AdvancedOptionsWidget(const Tp::ProtocolParameterList& parameters,
                               const QVariantMap &values = QVariantMap(),
                               QWidget *parent = 0);
    virtual ~AdvancedOptionsWidget();

    virtual QList<ProtocolParameterValue> parameterValues() const;
    virtual bool validateParameterValues();

private:
    Q_DISABLE_COPY(AdvancedOptionsWidget);

    class Private;
    Private * const d;
};


#endif // header guard

