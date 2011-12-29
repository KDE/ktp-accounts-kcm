/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2010 Collabora Ltd. <info@collabora.com>
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

#include "account-edit-widget.h"
#include "ui_account-edit-widget.h"

#include "abstract-account-parameters-widget.h"
#include "abstract-account-ui.h"
#include "parameter-edit-widget.h"
#include "plugin-manager.h"
#include "dictionary.h"
#include "feedback-widget.h"

#include <KLocale>
#include <KMessageWidget>

#include <TelepathyQt/Profile>
#include <QtCore/QList>
#include <QtGui/QCheckBox>


class AccountEditWidget::Private
{
public:
    Private()
            : accountUi(0), mainOptionsWidget(0)
    {
    }

    QString connectionManager;
    QString protocol;
    QString serviceName;

    QCheckBox *connectOnAdd;

    ParameterEditModel *parameterModel;
    AbstractAccountUi *accountUi;
    Ui::AccountEditWidget *ui;
    AbstractAccountParametersWidget *mainOptionsWidget;
};

AccountEditWidget::AccountEditWidget(const Tp::ProfilePtr &profile,
                                     ParameterEditModel *parameterModel,
                                     ConnectOnLoadType connectOnAddFlag,
                                     QWidget *parent)
        : QWidget(parent),
          d(new Private)
{
    // Set up the interface
    d->ui = new Ui::AccountEditWidget;
    d->ui->setupUi(this);

    d->parameterModel = parameterModel;
    d->serviceName = profile->serviceName();
    d->connectionManager = profile->cmName();
    d->protocol = profile->protocolName();

    connect(d->ui->advancedButton, SIGNAL(clicked()),
            this, SLOT(onAdvancedClicked()));

    FeedbackWidget *feedback = new FeedbackWidget();
    d->ui->verticalLayout->insertWidget(1, feedback);
    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)),
            feedback,
            SLOT(setMessage(QString,QString,KMessageWidget::MessageType)));
    connect(d->parameterModel,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)),
            feedback,
            SLOT(setMessage(QString,QString,KMessageWidget::MessageType)));

    d->ui->advancedButton->setIcon(KIcon(QLatin1String("configure")));
    //FIXME: Dictionary should not be needed anymore when distros ship profiles
    QString localizedName = Dictionary::instance()->string(profile->name());
    if (localizedName.isEmpty()) {
        localizedName = profile->name();
    }
    d->ui->titleLabel->setText(localizedName);
    d->ui->iconLabel->setText(QString());
    d->ui->iconLabel->setPixmap(KIcon(profile->iconName()).pixmap(32));

    if (connectOnAddFlag == doConnectOnAdd) {
        d->connectOnAdd = new QCheckBox(i18n("Connect when wizard is finished"), this);
        d->connectOnAdd->setChecked(true);
        d->ui->verticalLayout->addWidget(d->connectOnAdd);
    } else {
        d->connectOnAdd = 0;
    }

    loadWidgets();
}

AccountEditWidget::~AccountEditWidget()
{
    delete d->ui;
    delete d;
}

QString AccountEditWidget::errorMessage() const
{
    return d->mainOptionsWidget->errorMessage();
}

bool AccountEditWidget::validateParameterValues() const
{
    // the parameters handle by the advanced dialog are validated when the
    // dialog exits, so no need to validate them here
    return d->mainOptionsWidget->validateParameterValues();
}


void AccountEditWidget::loadWidgets()
{
    Tp::ProtocolParameterList mandatoryParameters;

    Q_FOREACH (const Tp::ProtocolParameter &parameter, d->parameterModel->parameters()) {
        if (parameter.isRequired()) {
            mandatoryParameters.append(parameter);
        }
    }

    // Get the AccountsUi for the plugin, and get the optional parameter widgets for it.
    d->accountUi = PluginManager::instance()->accountUiForProtocol(d->connectionManager,
                                                                   d->protocol,
                                                                   d->serviceName);

    // Create the custom UI or generic UI depending on available parameters.
    if (d->accountUi) {
        // UI does exist, set it up.
        d->mainOptionsWidget = d->accountUi->mainOptionsWidget(d->parameterModel,
                                                               this);
        //Widgets wrapped in a layout should not have double margins
        d->mainOptionsWidget->layout()->setContentsMargins(0, 0, 0, 0);
        d->ui->advancedButton->setVisible(d->accountUi->hasAdvancedOptionsWidget());
        d->ui->verticalLayout->insertWidget(2, d->mainOptionsWidget);

        // check if all the parameters the UI supports are available in the CM plugin
        // also verify if the UI handle all mandatory parameters
        QMap<QString, QVariant::Type> params = d->accountUi->supportedParameters();
        QMap<QString, QVariant::Type>::const_iterator paramIter = params.constBegin();
        while (paramIter != params.constEnd()) {
            Q_FOREACH (const Tp::ProtocolParameter &parameter, d->parameterModel->parameters()) {
                if ((parameter.name() == paramIter.key()) &&
                    (parameter.type() == paramIter.value())) {
                    mandatoryParameters.removeAll(parameter);
                }
            }
            ++paramIter;
        }

        // if a mandatory parameter is not being handled,
        // fall back to the generic interface.
        // FIXME: not sure this is the best way to handle the situation, but
        //        it is like this in a hope the case won't happen often
        if (!mandatoryParameters.isEmpty()) {
            delete d->mainOptionsWidget;
            d->mainOptionsWidget = 0;
        }
    }

    if (!d->mainOptionsWidget) {
        d->mainOptionsWidget = new ParameterEditWidget(
                d->parameterModel,
                this);
        d->ui->advancedButton->setVisible(false);
        d->ui->verticalLayout->insertWidget(2, d->mainOptionsWidget);
        d->ui->verticalLayout->setStretch(2, 1);
    }
}

QVariantMap AccountEditWidget::parametersSet() const
{
    d->mainOptionsWidget->submit();
    return d->parameterModel->parametersSet();
}

QStringList AccountEditWidget::parametersUnset() const
{
    d->mainOptionsWidget->submit();
    return d->parameterModel->parametersUnset();
}

void AccountEditWidget::onAdvancedClicked()
{
    if (!d->parameterModel->validateParameterValues()) {
        return;
    }

    QWeakPointer<KDialog> dialog = new KDialog(this);
    dialog.data()->setWindowTitle(i18n("Advanced Options"));

    AbstractAccountParametersWidget *advancedWidget;
    advancedWidget = d->accountUi->advancedOptionsWidget(d->parameterModel,
                                                         dialog.data());
    dialog.data()->setMainWidget(advancedWidget);

    // loop until the entered values are ok or the user cancels the dialog

    while (true) {
        if (dialog.data()->exec() == KDialog::Accepted) {
            advancedWidget->submit();
            // validate the parameter values
            if (!advancedWidget->validateParameterValues()) {
                continue;
            }
        }
        break;
    }

    if (!dialog.isNull()) {
        dialog.data()->deleteLater();
    }
}

ParameterEditModel* AccountEditWidget::parameterModel() const
{
    return d->parameterModel;
}

bool AccountEditWidget::connectOnAdd()
{
    if (d->connectOnAdd == 0) {
        return false;
    }
    else{
        return d->connectOnAdd->isChecked();
    }
}

#include "account-edit-widget.moc"
