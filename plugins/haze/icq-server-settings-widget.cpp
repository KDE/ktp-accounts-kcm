/*
 * This file is part of telepathy-accounts-kcm
 *
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

#include "icq-server-settings-widget.h"

#include <KDE/KLocale>
#include <KDE/KGlobal>
#include <KDE/KCharsets>

#include <KCMTelepathyAccounts/ParameterEditModel>

IcqServerSettingsWidget::IcqServerSettingsWidget(ParameterEditModel *model,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::IcqServerSettingsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("server"), QVariant::String, m_ui->serverLineEdit, m_ui->serverLabel);
    handleParameter(QLatin1String("port"), QVariant::Int, m_ui->portSpinBox, m_ui->portLabel);
    handleParameter(QLatin1String("use-ssl"), QVariant::Bool, m_ui->useSslCheckBox, 0);
    handleParameter(QLatin1String("allow-multiple-logins"), QVariant::Bool, m_ui->allowMultipleLoginsCheckBox, 0);
    handleParameter(QLatin1String("always-use-rv-proxy"), QVariant::Bool, m_ui->alwaysUseRvProxyCheckBox, 0);

    // update combo box for charset parameter
    Q_FOREACH (const QString &name, KGlobal::charsets()->descriptiveEncodingNames()) {
        m_ui->charsetComboBox->addItem(name);
    }

    const QString encName = parameterModel()->data(
        parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("charset"))),
        ParameterEditModel::ValueRole).toString();
    m_ui->charsetComboBox->setCurrentItem(KGlobal::charsets()->descriptionForEncoding(encName));
}

IcqServerSettingsWidget::~IcqServerSettingsWidget()
{
    delete m_ui;
}

void IcqServerSettingsWidget::submit()
{
    AbstractAccountParametersWidget::submit();

    const QString name = KGlobal::charsets()->encodingForName(m_ui->charsetComboBox->currentText());
    parameterModel()->setData(
        parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("charset"))),
        name, ParameterEditModel::ValueRole);
}

#include "icq-server-settings-widget.moc"
