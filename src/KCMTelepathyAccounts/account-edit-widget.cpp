/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2010 Collabora Ltd. <info@collabora.com>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2013 Daniele E. Domenichelli <ddomenichelli@kde.org>
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

#include <KLocalizedString>
#include <KMessageWidget>

#include <TelepathyQt/Profile>

#include <QList>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>

class AccountEditWidget::Private
{
public:
    Private()
            : accountUi(nullptr), mainOptionsWidget(nullptr)
    {
    }

    QString connectionManager;
    QString protocol;
    QString serviceName;
    QString displayName;
    QString defaultDisplayName;

    QCheckBox *connectOnAdd;

    ParameterEditModel *parameterModel;
    AbstractAccountUi *accountUi;
    Ui::AccountEditWidget *ui;
    AbstractAccountParametersWidget *mainOptionsWidget;
};

AccountEditWidget::AccountEditWidget(const Tp::ProfilePtr &profile,
                                     const QString &displayName,
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
    d->displayName = displayName;

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

    d->ui->advancedButton->setIcon(QIcon::fromTheme(QLatin1String("configure")));
    //FIXME: Dictionary should not be needed anymore when distros ship profiles
    QString localizedName = Dictionary::instance()->string(profile->name());
    if (localizedName.isEmpty()) {
        localizedName = profile->name();
    }
    d->ui->titleLabel->setText(localizedName);
    d->ui->iconLabel->setText(QString());
    d->ui->iconLabel->setPixmap(QIcon::fromTheme(profile->iconName()).pixmap(32));

    if (connectOnAddFlag == doConnectOnAdd) {
        d->connectOnAdd = new QCheckBox(i18n("Connect when wizard is finished"), this);
        d->connectOnAdd->setChecked(true);
        d->ui->verticalLayout->addWidget(d->connectOnAdd);
    } else {
        d->connectOnAdd = nullptr;
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
            d->mainOptionsWidget = nullptr;
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

    // Update the default display name after setting up the ui and updating all
    // the parameters so that it is generated accordingly.
    d->defaultDisplayName = defaultDisplayName();
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

    QPointer<QDialog> dialog = new QDialog(this);
    dialog.data()->setWindowTitle(i18n("Advanced Options"));

    AbstractAccountParametersWidget *advancedWidget;
    advancedWidget = d->accountUi->advancedOptionsWidget(d->parameterModel,
                                                         dialog.data());

    QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog.data());
    connect(dbb, SIGNAL(accepted()), dialog.data(), SLOT(accept()));
    connect(dbb, SIGNAL(rejected()), dialog.data(), SLOT(reject()));

    QVBoxLayout *layout = new QVBoxLayout(dialog.data());
    layout->addWidget(advancedWidget);
    layout->addWidget(dbb);
    dialog.data()->setLayout(layout);

    // loop until the entered values are ok or the user cancels the dialog

    while (true) {
        if (dialog.data()->exec() == QDialog::Accepted) {
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
    if (d->connectOnAdd == nullptr) {
        return false;
    }
    else{
        return d->connectOnAdd->isChecked();
    }
}

QString AccountEditWidget::defaultDisplayName() const
{
    return d->mainOptionsWidget->defaultDisplayName();
}

QString AccountEditWidget::displayName() const
{
    if (d->displayName.isEmpty()) {
        return d->mainOptionsWidget->defaultDisplayName();
    }

    return d->displayName;
}

bool AccountEditWidget::updateDisplayName()
{
    QString newDefaultDisplayName = defaultDisplayName();
    QString newDisplayName;
    // If the display name is empty or is the old default one the value is
    // cleared (empty displayName = use default one). If the display name
    // contains the old default value (probably the user added something, the
    // default part is replaced and the rest is not changed. Otherwise display
    // name was customized, therefore we leave it unchanged.
    if (d->displayName.isEmpty() || d->displayName == d->defaultDisplayName) {
        newDisplayName = newDefaultDisplayName;
    } else {
        newDisplayName = d->displayName;
        newDisplayName.replace(d->defaultDisplayName, newDefaultDisplayName);
    }

    d->defaultDisplayName = newDefaultDisplayName;
    if (d->displayName != newDisplayName) {
        d->displayName = newDisplayName;
        return true;
    }
    return false;
}
