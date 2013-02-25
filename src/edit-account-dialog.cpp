/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#include "edit-account-dialog.h"

#include <KTp/wallet-interface.h>
#include <KTp/pending-wallet.h>
#include <KTp/wallet-utils.h>

#include "KCMTelepathyAccounts/dictionary.h"
#include "KCMTelepathyAccounts/abstract-account-parameters-widget.h"
#include "KCMTelepathyAccounts/abstract-account-ui.h"
#include "KCMTelepathyAccounts/account-edit-widget.h"
#include "KCMTelepathyAccounts/parameter-edit-widget.h"
#include "KCMTelepathyAccounts/parameter-edit-model.h"
#include "KCMTelepathyAccounts/plugin-manager.h"

#include <KDebug>
#include <KLocale>
#include <KTabWidget>

#include <QtCore/QList>

#include <TelepathyQt/Account>
#include <TelepathyQt/ConnectionManager>
#include <TelepathyQt/PendingStringList>

class EditAccountDialog::Private
{
public:
    Private()
        :  widget(0), reconnectRequired(false), kwalletReady(false)
    {
    }

    Tp::AccountPtr account;
    AccountEditWidget *widget;
    bool reconnectRequired;
    bool kwalletReady;
};

EditAccountDialog::EditAccountDialog(const Tp::AccountPtr &account, QWidget *parent)
        : KDialog(parent),
          d(new Private)
{
    d->account = account;

    connect(KTp::WalletInterface::openWallet(), SIGNAL(finished(Tp::PendingOperation*)), SLOT(onWalletOpened(Tp::PendingOperation*)));

    setMinimumWidth(400);
}

EditAccountDialog::~EditAccountDialog()
{
    delete d;
}

void EditAccountDialog::onWalletOpened(Tp::PendingOperation *op)
{
    KTp::PendingWallet *walletOp = qobject_cast<KTp::PendingWallet*>(op);
    Q_ASSERT(walletOp);

    KTp::WalletInterface *walletInterface = walletOp->walletInterface();

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->account->protocolInfo();
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();
    QVariantMap parameterValues = d->account->parameters();

    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(this);
    parameterModel->addItems(parameters, d->account->profile()->parameters(), parameterValues);

    //update the parameter model with the password from kwallet (if applicable)
    Tp::ProtocolParameter passwordParameter = parameterModel->parameter(QLatin1String("password"));

    if (passwordParameter.isValid() && walletInterface->hasPassword(d->account)) {
        QModelIndex index = parameterModel->indexForParameter(passwordParameter);
        QString password = walletInterface->password(d->account);
        parameterModel->setData(index, password, Qt::EditRole);
    }

    // Set up the interface
    d->widget = new AccountEditWidget(d->account->profile(),
                                      d->account->displayName(),
                                      parameterModel,
                                      doNotConnectOnAdd,
                                      this);

    setMainWidget(d->widget);

    d->kwalletReady = true;
    show();
}

void EditAccountDialog::accept()
{
    if (!d->widget) {
        kWarning() << "missing d->widget, not saving parameters";
        return;
    }

    QVariantMap setParameters = d->widget->parametersSet();
    QStringList unsetParameters = d->widget->parametersUnset();

    // Check all pages of parameters pass validation.
    if (!d->widget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    //remove password from setParameters as this is now stored by kwallet instead
    setParameters.remove(QLatin1String("password"));
    unsetParameters.append(QLatin1String("password")); //remove the password from mission control

    Tp::PendingStringList *psl = d->account->updateParameters(setParameters, unsetParameters);

    kDebug() << "Set parameters:" << setParameters;
    kDebug() << "Unset parameters:" << unsetParameters;

    connect(psl,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onParametersUpdated(Tp::PendingOperation*)));
}

void EditAccountDialog::onParametersUpdated(Tp::PendingOperation *op)
{
    if (op->isError()) {
        // FIXME: Visual feedback in GUI to user.
        kWarning() << "Could not update parameters:" << op->errorName() << op->errorMessage();
        return;
    }

    Tp::PendingStringList *psl = qobject_cast<Tp::PendingStringList*>(op);

    Q_ASSERT(psl);
    if (!psl) {
        kWarning() << "Something  weird happened";
    }

    if (psl->result().size() > 0) {
        kDebug() << "The following parameters won't be updated until reconnection: " << psl->result();
        d->reconnectRequired = true;
    }

    QVariantMap values = d->widget->parametersSet();

    if (values.contains(QLatin1String("password"))) {
        KTp::WalletUtils::setAccountPassword(d->account, values[QLatin1String("password")].toString());
    } else {
        KTp::WalletUtils::setAccountPassword(d->account, QString());
    }

    if(d->widget->updateDisplayName()) {
        connect(d->account->setDisplayName(d->widget->displayName()),
                SIGNAL(finished(Tp::PendingOperation*)),
                SLOT(onDisplayNameUpdated(Tp::PendingOperation*)));
    } else {
        onFinished();
    }
}

void EditAccountDialog::onDisplayNameUpdated(Tp::PendingOperation *op)
{
    if (op->isError()) {
        // FIXME: Visual feedback in GUI to user.
        kWarning() << "Could not update display name:" << op->errorName() << op->errorMessage();
        return;
    }
    onFinished();
}

void EditAccountDialog::onFinished()
{
    Q_EMIT finished();

    if (d->reconnectRequired) {
        d->account->reconnect();
    }

    // set the dialog as accepted and exit
    done(KDialog::Accepted);
}

void EditAccountDialog::setVisible(bool visible)
{
    if (visible && d->kwalletReady) {
        KDialog::setVisible(visible);
        return;
    }

    KDialog::setVisible(false);
}

#include "edit-account-dialog.moc"
