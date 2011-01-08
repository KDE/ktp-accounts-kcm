/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
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

#ifndef KCM_TELEPATHY_ACCOUNTS_ACCOUNT_EDIT_WIDGET_H
#define KCM_TELEPATHY_ACCOUNTS_ACCOUNT_EDIT_WIDGET_H

#include "protocol-parameter-value.h"

#include <kdemacros.h>

#include <QWidget>

#include <TelepathyQt4/ConnectionManager>


class KDE_EXPORT AccountEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccountEditWidget(const Tp::ProtocolInfo &info,
                               const QVariantMap &parameterValues = QVariantMap(),
                               QWidget *parent = 0);
    virtual ~AccountEditWidget();

    virtual bool validateParameterValues() const;
    virtual QList<ProtocolParameterValue> parameterValues() const;

private Q_SLOTS:
    void onAdvancedClicked();

private:
    Q_DISABLE_COPY(AccountEditWidget);

    void loadWidgets();

    class Private;
    Private * const d;
};


#endif // header guard

