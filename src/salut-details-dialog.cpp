/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "salut-details-dialog.h"

#include "KCMTelepathyAccounts/parameter-edit-model.h"
#include "KCMTelepathyAccounts/account-edit-widget.h"
#include "KCMTelepathyAccounts/profile-item.h"

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/ConnectionManager>
#include <TelepathyQt4/ProfileManager>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingAccount>

class SalutDetailsDialog::Private
{
public:
    Private(SalutDetailsDialog* parent)
        : q(parent),
          widget(0)
    {
        kDebug();
    }

    SalutDetailsDialog *q;
    AccountEditWidget *widget;

    Tp::ProfilePtr profile;
};

SalutDetailsDialog::SalutDetailsDialog(const Tp::ProfileManagerPtr profileManager, const Tp::ConnectionManagerPtr connectionManager, QWidget *parent)
    : KDialog(parent),
      d(new Private(this))
{
    kDebug();

    setMinimumWidth(450);

    // Get the protocol's parameters and values.
    Tp::ProtocolInfo protocolInfo = connectionManager->protocol(QLatin1String("local-xmpp"));
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();

    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(this);
    d->profile = profileManager->profilesForCM("salut").first();

    Q_ASSERT(!d->profile.isNull());
    Q_ASSERT(d->profile->isValid());
    Q_ASSERT(d->profile->protocolName() == QLatin1String("local-xmpp"));
    if(d->profile.isNull() || !d->profile->isValid() || d->profile->protocolName() != QLatin1String("local-xmpp"))
    {
        kWarning() << "Something went wrong with telepathy salut";
    }

    parameterModel->addItems(parameters, d->profile->parameters());

    // Set up the interface
    d->widget = new AccountEditWidget(d->profile,
                                      parameterModel,
                                      doNotConnectOnAdd,
                                      this);
    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)),
            d->widget,
            SIGNAL(feedbackMessage(QString,QString,KTitleWidget::MessageType)));
    setMainWidget(d->widget);
}

SalutDetailsDialog::~SalutDetailsDialog()
{
    delete d;
}

void SalutDetailsDialog::accept()
{
    Q_EMIT dialogAccepted(d->widget->parametersSet());
    KDialog::accept();
}

#include "salut-details-dialog.moc"
