/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "KCMTelepathyAccounts/abstract-account-parameters-widget.h"
#include "KCMTelepathyAccounts/abstract-account-ui.h"
#include "KCMTelepathyAccounts/account-edit-widget.h"
#include "KCMTelepathyAccounts/parameter-edit-widget.h"
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
            : item(0), widget(0)
    {
        kDebug();
    }

    AccountItem *item;
    AccountEditWidget *widget;
};

EditAccountDialog::EditAccountDialog(AccountItem *item, QWidget *parent)
        : KDialog(parent),
          d(new Private)
{
    kDebug();

    d->item = item;

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = d->item->account()->protocolInfo();
    QVariantMap parameterValues = d->item->account()->parameters();

    // Set up the interface
    d->widget = new AccountEditWidget(protocolInfo,
                                      parameterValues,
                                      this);
    setMainWidget(d->widget);
//    resize(400, 480);
}

EditAccountDialog::~EditAccountDialog()
{
    kDebug();
}

void EditAccountDialog::accept()
{
    kDebug();

    // Check all pages of parameters pass validation.
    if (!d->widget->validateParameterValues()) {
        kDebug() << "A widget failed parameter validation. Not accepting wizard.";
        return;
    }

    // Get the mandatory parameters.
    QList<ProtocolParameterValue> parameterValues;
    parameterValues = d->widget->parameterValues();

    QVariantMap setParameters;
    QStringList unsetParameters;

    foreach(const ProtocolParameterValue &ppv, parameterValues)
    {
        if (ppv.shouldSave()) {
            setParameters.insert(ppv.name(), ppv.value());
        }
        else {
            unsetParameters.append(ppv.name());
        }
    }

    // kDebug() << "Set parameters:" << parameters;
    // kDebug() << "Unset parameters:" << unsetParameters;

    Tp::PendingStringList *psl = d->item->account()->updateParameters(setParameters, unsetParameters);

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

    emit finished();

    // set the dialog as accepted and exit
    done(KDialog::Accepted);
}

#include "edit-account-dialog.moc"

