/*
 * This file is part of telepathy-accounts-kcm
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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_HAZE_SKYPE_SERVER_SETTINGS_WIDGET_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_HAZE_SKYPE_SERVER_SETTINGS_WIDGET_H

#include "ui_skype-advanced-settings-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

class SkypeAdvancedSettingsWidget : public AbstractAccountParametersWidget
{
    Q_OBJECT

public:
    explicit SkypeAdvancedSettingsWidget(ParameterEditModel *model, QWidget *parent = nullptr);
    ~SkypeAdvancedSettingsWidget() override;

private:
    Q_DISABLE_COPY(SkypeAdvancedSettingsWidget)
    Ui::SkypeAdvancedSettingsWidget *m_ui;
};


#endif // header guard
