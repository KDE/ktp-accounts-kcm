/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
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

#include "profile-select-widget.h"

#include "profile-item.h"
#include "profile-list-model.h"

#include "ui_profile-select-widget.h"

#include <KDebug>

#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/ProfileManager>
#include <TelepathyQt4/Feature>

class ProfileSelectWidget::Private
{
public:
    Private()
     : profileManager(0),
       ui(0),
       model(0)
    {
        kDebug();
    }

    Tp::ProfileManagerPtr profileManager;
    Ui::ProfileSelectWidget *ui;
    ProfileListModel *model;
};

ProfileSelectWidget::ProfileSelectWidget(QWidget *parent)
 : QWidget(parent),
   d(new Private)
{
    kDebug();

    // Set up the widget
    d->model = new ProfileListModel(this);

    d->ui = new Ui::ProfileSelectWidget;
    d->ui->setupUi(this);
    d->ui->profileListView->setModel(d->model);

    connect(d->ui->profileListView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            SLOT(onSelectionChanged(const QItemSelection &)));
    connect(d->ui->profileListView,
            SIGNAL(doubleClicked(QModelIndex)),
            SIGNAL(profileDoubleClicked()));

    d->profileManager = Tp::ProfileManager::create(QDBusConnection::sessionBus());

    // Until all distros ship correct profile files, we should fake them
    connect(d->profileManager.data()->becomeReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onProfileManagerReady(Tp::PendingOperation*)));
}

ProfileSelectWidget::~ProfileSelectWidget()
{
    kDebug();

    delete d->ui;
    delete d;
}

void ProfileSelectWidget::onProfileManagerReady(Tp::PendingOperation *op)
{
    kDebug();

    // Check the pending operation completed successfully.
    if (op->isError()) {
        kDebug() << "becomeReady() failed:" << op->errorName() << op->errorMessage();
        return;
    }

    d->model->setProfileManager(d->profileManager);
}

// Return the selected ProfileItem or 0 if nothing is selected.
ProfileItem *ProfileSelectWidget::selectedProfile()
{
    kDebug();

    // Get the indexes of the selected items from the view
    QModelIndexList selectedIndexes = d->ui->profileListView->selectionModel()->selectedIndexes();

    // If more than 1 profile is selected (shouldn't be possible, but just in case) error.
    if (selectedIndexes.size() > 1) {
        kWarning() << "More than 1 profile is selected.";
        return 0;
    }

    // If no indexes are selected, return 0.
    if (selectedIndexes.size() == 0) {
        return 0;
    }

    // 1 index is selected. Return the Pr for that.
    return d->model->itemForIndex(selectedIndexes.at(0));
}

void ProfileSelectWidget::onSelectionChanged(const QItemSelection &selected)
{
    kDebug();

    Q_EMIT profileGotSelected(!selected.isEmpty());
}


#include "profile-select-widget.moc"

