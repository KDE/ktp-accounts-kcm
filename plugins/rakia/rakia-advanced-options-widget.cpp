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

#include "rakia-advanced-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

#include <KDebug>

RakiaAdvancedOptionsWidget::RakiaAdvancedOptionsWidget(ParameterEditModel* model, QWidget* parent)
    : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::RakiaAdvancedOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("auth-user"),  QVariant::String,
                    m_ui->authUserLineEdit,
                    m_ui->authUserLabel);
    handleParameter(QLatin1String("registrar"),  QVariant::String,
                    m_ui->registrarLineEdit,
                    m_ui->registrarLabel);

    handleParameter(QLatin1String("proxy-host"), QVariant::String,
                    m_ui->proxyLineEdit,
                    m_ui->proxyLabel);
    handleParameter(QLatin1String("port"),       QVariant::UInt,
                    m_ui->portSpinBox,
                    m_ui->portLabel);
    handleParameter(QLatin1String("transport"),  QVariant::String,
                    m_ui->transportComboBox,
                    m_ui->transportLabel);

    handleParameter(QLatin1String("stun-server"),        QVariant::String,
                    m_ui->stunServerLineEdit,
                    m_ui->stunServerLabel);
    handleParameter(QLatin1String("stun-port"),          QVariant::UInt,
                    m_ui->stunPortSpinBox,
                    m_ui->stunPortLabel);

    handleParameter(QLatin1String("loose-routing"),      QVariant::Bool,
                    m_ui->looseRoutingCheckBox,
                    m_ui->looseRoutingLabel);
    handleParameter(QLatin1String("discover-binding"),   QVariant::Bool,
                    m_ui->discoverBindingCheckBox,
                    m_ui->discoverBindingLabel);

    handleParameter(QLatin1String("keepalive-mechanism"), QVariant::String,
                    m_ui->keepaliveMechanismComboBox,
                    m_ui->keepaliveMechanismLabel);
    handleParameter(QLatin1String("keepalive-interval"), QVariant::UInt,
                    m_ui->keepaliveIntervalNumInput,
                    m_ui->keepaliveIntervalLabel);

    kDebug() << parameterModel()->data(
                 parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("discover-stun"))),
                 ParameterEditModel::ValueRole);

    // enable/disable the stunGroupBox if discover-stun is false/true
    m_ui->stunGroupBox->setChecked(
        !parameterModel()->data(
            parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("discover-stun"))),
            ParameterEditModel::ValueRole).toBool()
    );

    // enable/disable keepalive-interval depending on keepalive-mechanism
    connect(m_ui->keepaliveMechanismComboBox,
            SIGNAL(currentIndexChanged(int)),
            SLOT(updateKeepalive(int))
           );

    // enable/disable keepalive-interval depending on current keepalive-mechanism
    updateKeepalive(m_ui->keepaliveMechanismComboBox->currentIndex());
}

void RakiaAdvancedOptionsWidget::updateKeepalive(int index)
{
    kDebug() << index;
    switch (index) {
    case 0:
    case 3:
    case 4:
        m_ui->keepaliveIntervalNumInput->setEnabled(false);
        break;
    default:
        m_ui->keepaliveIntervalNumInput->setEnabled(true);
        break;
    }
}

void RakiaAdvancedOptionsWidget::submit()
{
    // the gui does not use the telepathy-sofiasip/rakia parameter disvover-stun directly.
    // if the goupbox is enabled, discover-stun is disabled and vice versa.
    // TODO: the telepathy-sofiasip/rakia is buggy, atm till 0.6.6 and 0.7.1, see
    // https://bugs.freedesktop.org/show_bug.cgi?id=34227 for details
    kDebug() << "ok, we're saving now, we need to do some magic!";
    parameterModel()->setData(
        parameterModel()->indexForParameter(parameterModel()->parameter(QLatin1String("discover-stun"))),
        !m_ui->stunGroupBox->isChecked(),
        ParameterEditModel::ValueRole);

    AbstractAccountParametersWidget::submit();
}

RakiaAdvancedOptionsWidget::~RakiaAdvancedOptionsWidget()
{
    delete m_ui;
}

#include "rakia-advanced-options-widget.moc"
