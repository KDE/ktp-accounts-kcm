/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_GABBLE_ACCOUNT_PARAMETERS_WIDGET_FACEBOOK_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_GABBLE_ACCOUNT_PARAMETERS_WIDGET_FACEBOOK_H

#include "ui_main-options-widget-facebook.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

class MainOptionsWidgetFacebook : public AbstractAccountParametersWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(MainOptionsWidgetFacebook)

public:
    explicit MainOptionsWidgetFacebook(ParameterEditModel *model,
                               QWidget *parent = nullptr);
    ~MainOptionsWidgetFacebook() override;

    void submit() override;
    bool validateParameterValues() override;
    QString defaultDisplayName() const override;
private:
    Ui::MainOptionsWidgetFacebook *m_ui;
};

#endif // header guard

