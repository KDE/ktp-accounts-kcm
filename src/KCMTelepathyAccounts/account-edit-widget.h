/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2010 Collabora Ltd. <info@collabora.com>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#ifndef KCM_TELEPATHY_ACCOUNTS_ACCOUNT_EDIT_WIDGET_H
#define KCM_TELEPATHY_ACCOUNTS_ACCOUNT_EDIT_WIDGET_H

#include "kcm_telepathy_accounts_export.h"

#include <QtGui/QWidget>
#include <KMessageWidget>

#include <TelepathyQt/ConnectionManager>
#include "parameter-edit-model.h"

enum ConnectOnLoadType{
    doConnectOnAdd,
    doNotConnectOnAdd
};

class KCM_TELEPATHY_ACCOUNTS_EXPORT AccountEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccountEditWidget(const Tp::ProfilePtr &info,
                               ParameterEditModel *parameterModel,
                               ConnectOnLoadType connectOnAddFlag = doNotConnectOnAdd,
                               QWidget *parent = 0);
    virtual ~AccountEditWidget();

    QString errorMessage() const;
    virtual bool validateParameterValues() const;
    bool connectOnAdd();

    virtual QVariantMap parametersSet() const;
    virtual QStringList parametersUnset() const;

    virtual QString defaultDisplayName() const;
    virtual QString displayName() const;
    virtual void setDisplayName(const QString &displayName);

protected:
    ParameterEditModel *parameterModel() const;

private Q_SLOTS:
    void onAdvancedClicked();
    void onDefaultDisplayNameChanged(const QString &oldDefaultDisplayName,
                                     const QString &newDefaultDisplayName);

Q_SIGNALS:
    void feedbackMessage(const QString &text, const QString &comment, KMessageWidget::MessageType);
    void displayNameChanged(const QString &oldDisplayName,
                            const QString &newDisplayName);
    void defaultDisplayNameChanged(const QString &oldDefaultDisplayName,
                                   const QString &newDefaultDisplayName);

private:
    Q_DISABLE_COPY(AccountEditWidget);

    void loadWidgets();

    class Private;
    Private * const d;
};


#endif // header guard

