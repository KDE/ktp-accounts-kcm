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

#ifndef KCMTELEPATHYACCOUNTS_PLUGIN_SUNSHINE_ACCOUNT_PARAMETERS_WIDGET_H
#define KCMTELEPATHYACCOUNTS_PLUGIN_SUNSHINE_ACCOUNT_PARAMETERS_WIDGET_H

#include "ui_sunshine-main-options-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

class SunshineMainOptionsWidget : public AbstractAccountParametersWidget
{
    Q_OBJECT

public:
    explicit SunshineMainOptionsWidget(ParameterEditModel *model,
                                    QWidget *parent = 0);
    virtual ~SunshineMainOptionsWidget();

private:
    Q_DISABLE_COPY(SunshineMainOptionsWidget);
    Ui::SunshineMainOptionsWidget *m_ui;
};

#endif // header guard
