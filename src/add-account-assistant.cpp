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

#include "add-account-assistant.h"

#include "protocol-select-widget.h"

#include <KDebug>
#include <KLocale>
#include <KPageWidgetItem>
#include <KTabWidget>

class AddAccountAssistant::Private
{
public:
    Private()
     : pageOne(0)
    {
        kDebug();
    }

    ProtocolSelectWidget *widgetOne;
    KTabWidget *widgetTwo;
    KPageWidgetItem *pageOne;
    KPageWidgetItem *pageTwo;
};

AddAccountAssistant::AddAccountAssistant(QWidget *parent)
 : KAssistantDialog(parent),
   d(new Private)
{
    kDebug();

    // Set up the pages of the Assistant.
    d->widgetOne = new ProtocolSelectWidget(this);
    d->pageOne = new KPageWidgetItem(d->widgetOne);
    d->pageOne->setHeader(i18n("Step 1: Select an Instant Messaging Network."));
    d->widgetTwo = new KTabWidget(this);
    d->pageTwo = new KPageWidgetItem(d->widgetTwo);
    d->pageTwo->setHeader(i18n("Step 2: Fill in the required Parameters."));

    addPage(d->pageOne);
    addPage(d->pageTwo);
}

AddAccountAssistant::~AddAccountAssistant()
{
    kDebug();

    delete d;
}

void AddAccountAssistant::back()
{
    kDebug();

    // TODO: Implement me!
}

void AddAccountAssistant::next()
{
    kDebug();

    // Check which page we are on.
    if (currentPage() == d->pageOne) {
        kDebug() << "Current page: Page 1.";
        // Page 1

        // Check if the protocol is selected. If it is, continue to the next page.
        if (d->widgetOne->selectedProtocol()) {
            kDebug() << "Protocol is selected. Continue to the next page.";
            KAssistantDialog::next();
        }
    }
}

void AddAccountAssistant::accept()
{
    kDebug();

    // TODO: Implement me!
}

void AddAccountAssistant::reject()
{
    kDebug();

    // Emit a signal to tell the assistant launcher that it was cancelled.
    Q_EMIT cancelled();

    // Close the assistant
    KAssistantDialog::reject();
}

