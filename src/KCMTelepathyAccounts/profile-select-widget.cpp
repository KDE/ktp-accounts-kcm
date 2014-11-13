/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <info@collabora.com>
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

#include <QDebug>

#include <QSortFilterProxyModel>
#include <QItemSelectionModel>

class ProfileSelectWidget::Private
{
public:
    Private()
     : ui(0),
       sortModel(0),
       sourceModel(0)
    {
    }

    Ui::ProfileSelectWidget *ui;
    QSortFilterProxyModel *sortModel;
    ProfileListModel *sourceModel;
};

ProfileSelectWidget::ProfileSelectWidget(ProfileListModel *profileListModel, QWidget *parent, bool enableSalut)
 : QWidget(parent),
   d(new Private)
{
    (void)profileListModel;
    // Set up the models
    d->sourceModel = profileListModel;
    d->sortModel = new QSortFilterProxyModel(this);
    d->sortModel->setSourceModel(d->sourceModel);
    d->sortModel->sort(0, Qt::AscendingOrder);
    d->sortModel->setDynamicSortFilter(true);
    d->sortModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    // Here we filter out salut and not local-xmpp because it might want to be
    // able to see local-xmpp using haze later.
    if (!enableSalut) {
        d->sortModel->setFilterRole(ProfileListModel::ProfileCmNameRole);
        d->sortModel->setFilterRegExp(QLatin1String("^((?!salut).)*$"));
    }

    // Set up the widget
    d->ui = new Ui::ProfileSelectWidget;
    d->ui->setupUi(this);
    d->ui->profileListView->setModel(d->sortModel);


    connect(d->ui->profileListView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(onSelectionChanged(QItemSelection)));
    connect(d->ui->profileListView,
            SIGNAL(doubleClicked(QModelIndex)),
            SIGNAL(profileChosen()));
}

ProfileSelectWidget::~ProfileSelectWidget()
{
    delete d->ui;
    delete d;
}

// Return the selected ProfileItem or 0 if nothing is selected.
ProfileItem *ProfileSelectWidget::selectedProfile()
{
    // Get the indexes of the selected items from the view
    QModelIndexList selectedIndexes = d->ui->profileListView->selectionModel()->selectedIndexes();

    // If more than 1 profile is selected (shouldn't be possible, but just in case) error.
    if (selectedIndexes.size() > 1) {
        qWarning() << "More than 1 profile is selected.";
        return 0;
    }

    // If no indexes are selected, return 0.
    if (selectedIndexes.size() == 0) {
        return 0;
    }

    // 1 index is selected. Return the Pr for that.
    QItemSelection sourceSelection = d->sortModel->mapSelectionToSource(d->ui->profileListView->selectionModel()->selection());

    return d->sourceModel->itemForIndex(sourceSelection.indexes().first());
}

void ProfileSelectWidget::onSelectionChanged(const QItemSelection &selected)
{
    Q_EMIT profileSelected(!selected.isEmpty());
}


#include "profile-select-widget.moc"
