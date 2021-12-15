/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
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

#include "simple-profile-select-widget.h"

#include "profile-item.h"
#include "profile-list-model.h"
#include "dictionary.h"

#include "ui_simple-profile-select-widget.h"

#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ProfileManager>
#include <TelepathyQt/Feature>

#include <QDebug>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QCommandLinkButton>

class SimpleProfileSelectWidget::Private
{
public:
    Private()
     : profileListModel(nullptr),
       ui(nullptr),
       signalMapper(nullptr)
    {
    }

    ProfileListModel *profileListModel;
    Ui::SimpleProfileSelectWidget *ui;
    QSignalMapper *signalMapper;
    QString profileName;
};

SimpleProfileSelectWidget::SimpleProfileSelectWidget(ProfileListModel *profileListModel, QWidget *parent)
 : QWidget(parent),
   d(new Private)
{
    // Set up the models
    d->profileListModel = profileListModel;
    d->signalMapper = new QSignalMapper(this);

    // Set up the widget
    d->ui = new Ui::SimpleProfileSelectWidget;
    d->ui->setupUi(this);

    // Add buttons to the SignalMapper
    d->signalMapper->setMapping(d->ui->buttonJabber,   QLatin1String("jabber"));
    d->signalMapper->setMapping(d->ui->buttonKDETalk,  QLatin1String("kde-talk"));
    d->signalMapper->setMapping(d->ui->buttonGTalk,    QLatin1String("google-talk"));
    d->signalMapper->setMapping(d->ui->buttonFacebook, QLatin1String("facebook"));

    // Connect buttons to the SignalMapper
    connect(d->ui->buttonJabber,   SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(d->ui->buttonKDETalk,  SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(d->ui->buttonGTalk,    SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(d->ui->buttonFacebook, SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(d->ui->buttonOthers,   SIGNAL(clicked()), this,            SIGNAL(othersChosen()));

    connect(d->signalMapper,SIGNAL(mapped(QString)),
            this,           SLOT(onProfileClicked(QString)));

    // Disable everything until Tp::ProfileManager is ready
    d->ui->verticalLayout->setEnabled(false);
}

SimpleProfileSelectWidget::~SimpleProfileSelectWidget()
{
    delete d->ui;
    delete d;
}

// Return the selected ProfileItem or 0 if nothing is selected.
ProfileItem *SimpleProfileSelectWidget::selectedProfile()
{
   return d->profileListModel->itemForService(d->profileName);
}

// FIXME: before we proceed here, we should check if there's everything installed we need.
void SimpleProfileSelectWidget::onProfileClicked(QString profileName)
{
    d->profileName = profileName;

    qDebug() << "Selected profile: " << d->profileName;

    Q_EMIT profileChosen();
}
