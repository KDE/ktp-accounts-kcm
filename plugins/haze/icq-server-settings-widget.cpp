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
    initEncodings();
    const QString encName = parameterModel()->data(
        parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("charset"))),
        ParameterEditModel::ValueRole).toString();
    QTextCodec *codec = QTextCodec::codecForName(encName.toLatin1());
    int mib = 106; // UTF-8
    if (!codec) {
        codec = KGlobal::locale()->codecForEncoding();
    }
    if (codec) {
        mib = codec->mibEnum();
    }
    m_ui->charsetComboBox->setCurrentIndex(m_ui->charsetComboBox->findData(mib));
}

IcqServerSettingsWidget::~IcqServerSettingsWidget()
{
    delete m_ui;
}

void IcqServerSettingsWidget::submit()
{
    AbstractAccountParametersWidget::submit();

    QTextCodec *codec = QTextCodec::codecForMib(m_ui->charsetComboBox->itemData(
        m_ui->charsetComboBox->currentIndex()).toInt());
    if (!codec) {
        codec = QTextCodec::codecForMib(106); // UTF-8
    }

    parameterModel()->setData(
        parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("charset"))),
        codec->name(), ParameterEditModel::ValueRole);
}


void IcqServerSettingsWidget::initEncodings()
{
    m_ui->charsetComboBox->addItem(i18n("Big5"), 2026);
    m_ui->charsetComboBox->addItem(i18n("Big5-HKSCS"), 2101);
    m_ui->charsetComboBox->addItem(i18n("euc-JP Japanese"), 18);
    m_ui->charsetComboBox->addItem(i18n("euc-KR Korean"), 38);
    m_ui->charsetComboBox->addItem(i18n("GB-2312 Chinese"), 57);
    m_ui->charsetComboBox->addItem(i18n("GBK Chinese"), 113);
    m_ui->charsetComboBox->addItem(i18n("GB18030 Chinese"), 114);

    m_ui->charsetComboBox->addItem(i18n("JIS Japanese"), 16);
    m_ui->charsetComboBox->addItem(i18n("Shift-JIS Japanese"), 17);

    m_ui->charsetComboBox->addItem(i18n("KOI8-R Russian"), 2084);
    m_ui->charsetComboBox->addItem(i18n("KOI8-U Ukrainian"), 2088);

    m_ui->charsetComboBox->addItem(i18n("ISO-8859-1 Western"), 4);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-2 Central European"), 5);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-3 Central European"), 6);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-4 Baltic"), 7);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-5 Cyrillic"), 8);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-6 Arabic"), 9);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-7 Greek") ,10);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-8 Hebrew, visually ordered"), 11 );
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-8-I Hebrew, logically ordered"), 85);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-9 Turkish"), 12);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-10"), 13);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-13"), 109);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-14"), 110);
    m_ui->charsetComboBox->addItem(i18n("ISO-8859-15 Western"), 111);

    m_ui->charsetComboBox->addItem(i18n("Windows-1250 Central European"), 2250);
    m_ui->charsetComboBox->addItem(i18n("Windows-1251 Cyrillic"), 2251);
    m_ui->charsetComboBox->addItem(i18n("Windows-1252 Western"), 2252);
    m_ui->charsetComboBox->addItem(i18n("Windows-1253 Greek"), 2253);
    m_ui->charsetComboBox->addItem(i18n("Windows-1254 Turkish"), 2254);
    m_ui->charsetComboBox->addItem(i18n("Windows-1255 Hebrew"), 2255);
    m_ui->charsetComboBox->addItem(i18n("Windows-1256 Arabic"), 2256);
    m_ui->charsetComboBox->addItem(i18n("Windows-1257 Baltic"), 2257);
    m_ui->charsetComboBox->addItem(i18n("Windows-1258 Viet Nam"), 2258);

    m_ui->charsetComboBox->addItem(i18n("IBM 850"), 2009);
    m_ui->charsetComboBox->addItem(i18n("IBM 866"), 2085);

    m_ui->charsetComboBox->addItem(i18n("TIS-620 Thai"), 2259);

    m_ui->charsetComboBox->addItem(i18n("UTF-8 Unicode"), 106);
    m_ui->charsetComboBox->addItem(i18n("UTF-16 Unicode"), 1015);
}


#include "icq-server-settings-widget.moc"
