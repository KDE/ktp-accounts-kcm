/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include <TelepathyQt4/ConnectionManager>
#include <TelepathyQt4/PendingStringList>

class EditAccountDialog::Private
{
public:
    Private()
            : item(0), widget(0), reconnectRequired(false)
    {
        kDebug();
    }

    AccountItem *item;
    AccountEditWidget *widget;
    bool reconnectRequired;
};

EditAccountDialog::EditAccountDialog(AccountItem *item, QWidget *parent)
        : KDialog(parent),
          d(new Private)
{
    kDebug();

    d->item = item;

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->item->account()->protocolInfo();
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();
    QVariantMap parameterValues = d->item->account()->parameters();

    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(this);
    parameterModel->addItems(parameters, d->item->account()->profile()->parameters(), parameterValues);

    // Set up the interface
    d->widget = new AccountEditWidget(d->item->account()->profile(),
                                      parameterModel,
                                      doNotConnectOnAdd,
                                      this);
    setMainWidget(d->widget);
    setMinimumWidth(400);
}

EditAccountDialog::~EditAccountDialog()
{
    kDebug();

    delete d;
}

void EditAccountDialog::accept()
{
    kDebug();

    QVariantMap setParameters = d->widget->parametersSet();
    QStringList unsetParameters = d->widget->parametersUnset();

    // Check all pages of parameters pass validation.
    if (!d->widget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    Tp::PendingStringList *psl = d->item->account()->updateParameters(setParameters, unsetParameters);

    // We don't want to print on the screen our passwords therefore we hide it before printing
    if (setParameters.contains(QLatin1String("password")))
    {
        setParameters[QLatin1String("password")] = QLatin1String("**********");
    }
    kDebug() << "Set parameters:" << setParameters;
    kDebug() << "Unset parameters:" << unsetParameters;

    connect(psl,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onParametersUpdated(Tp::PendingOperation*)));
}

void EditAccountDialog::onParametersUpdated(Tp::PendingOperation *op)
{
    kDebug();

    if (op->isError()) {
        // FIXME: Visual feedback in GUI to user.
        kWarning() << "Could not update parameters:" << op->errorName() << op->errorMessage();
        return;
    }

    Tp::PendingStringList *psl = qobject_cast<Tp::PendingStringList*>(op);

    Q_ASSERT(psl);
    if (!psl)
    {
        kWarning() << "Something  weird happened";
    }

    if (psl->result().size() > 0) {
        kDebug() << "The following parameters won't be updated until reconnection: " << psl->result();
        d->reconnectRequired = true;
    }

    QVariantMap values = d->widget->parametersSet();
    // FIXME: Ask the user to submit a Display Name

    QString displayName;
    if (values.contains("account")) {
        displayName = values["account"].toString();
    }
    else {
        displayName = d->item->account()->profile()->protocolName();
    }

    Tp::PendingOperation *dnop = d->item->account()->setDisplayName(displayName);

    connect(dnop,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onDisplayNameUpdated(Tp::PendingOperation*)));
}

void EditAccountDialog::onDisplayNameUpdated(Tp::PendingOperation *op)
{
    kDebug();

    if (op->isError()) {
        // FIXME: Visual feedback in GUI to user.
        kWarning() << "Could not update display name:" << op->errorName() << op->errorMessage();
        return;
    }

    emit finished();

    if (d->reconnectRequired) {
        d->item->account()->reconnect();
    }

    // set the dialog as accepted and exit
    done(KDialog::Accepted);
}

#include "edit-account-dialog.moc"

