/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson. <kde@davidedmundson.co.uk>
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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_GABBLE_ACCOUNT_PARAMETERS_WIDGET_GOOGLETALK_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_GABBLE_ACCOUNT_PARAMETERS_WIDGET_GOOGLETALK_H

#include "ui_main-options-widget-googletalk.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

class MainOptionsWidgetGoogleTalk : public AbstractAccountParametersWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(MainOptionsWidgetGoogleTalk)


public:
    explicit MainOptionsWidgetGoogleTalk(ParameterEditModel *model,
                               QWidget *parent = 0);
    ~MainOptionsWidgetGoogleTalk() override;

    QString defaultDisplayName() const override;
private:
    Ui::MainOptionsWidgetGoogleTalk *m_ui;
};

#endif // header guard

