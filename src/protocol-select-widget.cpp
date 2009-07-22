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

#include "protocol-select-widget.h"

#include "ui_protocol-select-widget.h"

#include <KDebug>

#include <TelepathyQt4/ConnectionManager>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingStringList>

class ProtocolSelectWidget::Private
{
public:
    Private()
     : ui(0)
    {
        kDebug();
    }

    Ui::ProtocolSelectWidget *ui;
};

ProtocolSelectWidget::ProtocolSelectWidget(QWidget *parent)
 : QWidget(parent),
   d(new Private)
{
    kDebug();

    // Set up the widget
    d->ui = new Ui::ProtocolSelectWidget;
    d->ui->setupUi(this);

    // Load the list of all installed Telepathy Connection Managers Asynchronously
    QTimer::singleShot(0, this, SLOT(getConnectionManagerList()));
}

ProtocolSelectWidget::~ProtocolSelectWidget()
{
    kDebug();

    delete d;
}

void ProtocolSelectWidget::getConnectionManagerList()
{
    kDebug();

    // Ask TpQt4 for the list of all installed Connection Managers.
    Tp::PendingStringList *psl = Tp::ConnectionManager::listNames();

    connect(psl,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onConnectionManagerListGot(Tp::PendingOperation*)));
}

void ProtocolSelectWidget::onConnectionManagerListGot(Tp::PendingOperation *op)
{
    kDebug();

    // Check the operation completed successfully.
    if (op->isError()) {
        kWarning() << "Operation failed:" << op->errorName() << op->errorMessage();
        return;
    }

    // Check the operation we were passed is of the correct type.
    Tp::PendingStringList *psl = qobject_cast<Tp::PendingStringList*>(op);
    if (!psl) {
        kWarning() << "Operation is not of type PendingStringList.";
        return;
    }

    foreach (QString cmName, psl->result()) {
        // TODO: Add that CM to the protocol-list-model
    }
}

