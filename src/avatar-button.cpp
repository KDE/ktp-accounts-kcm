/*
 * Button representing user's Avatar
 *
 * Copyright (C) 2011  Martin Klapetek <martin.klapetek@gmail.com>
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

#include "avatar-button.h"

#include <QWidgetAction>

#include <KFileDialog>
#include <KMenu>
#include <KLocalizedString>
#include <KMessageBox>

#include "fetch-avatar-job.h"

AvatarButton::AvatarButton(QWidget *parent)
    : QToolButton(parent)
{
    KMenu *menu = new KMenu(this);

    setPopupMode(QToolButton::InstantPopup);

    setIconSize(QSize(64,64));

    menu->addAction(KIcon("document-open-folder"), i18n("Load from file..."), this, SLOT(onLoadAvatarFromFile()));
    menu->addAction(KIcon("edit-clear"),i18n("Clear Avatar"), this, SLOT(onClearAvatar()));


    setMenu(menu);
}

AvatarButton::~AvatarButton()
{

}

void AvatarButton::setAvatar(const Tp::Avatar &avatar) {
    m_avatar = avatar;

    if (! avatar.avatarData.isNull()) {
        KIcon avatarIcon;
        QPixmap avatarPixmap = QPixmap::fromImage(QImage::fromData(avatar.avatarData));
        //scale oversized avatars to fit, but don't stretch smaller avatars
        avatarIcon.addPixmap(avatarPixmap.scaled(iconSize().boundedTo(avatarPixmap.size()), Qt::KeepAspectRatio));
        setIcon(avatarIcon);
    } else {
        setIcon(KIcon("im-user"));
    }
}

Tp::Avatar AvatarButton::avatar() const {
    return m_avatar;
}


void AvatarButton::onLoadAvatarFromFile()
{
    KUrl fileUrl = KFileDialog::getImageOpenUrl(KUrl(), this,
                                                i18n("Please choose your avatar"));

    if (!fileUrl.isEmpty()) {
        FetchAvatarJob *job = new FetchAvatarJob(fileUrl, this);

        connect(job, SIGNAL(result(KJob*)),
                this, SLOT(onAvatarFetched(KJob*)));

        job->start();
    } else {
        return;
    }
}

void AvatarButton::onClearAvatar()
{
    setAvatar(Tp::Avatar());
    Q_EMIT avatarChanged();
}

void AvatarButton::onAvatarFetched(KJob *job)
{
    if (job->error()) {
        KMessageBox::error(this, job->errorString());
        return;
    } else {
        FetchAvatarJob *fetchJob = qobject_cast< FetchAvatarJob* >(job);

        Q_ASSERT(fetchJob);
        setAvatar(fetchJob->avatar());
        Q_EMIT avatarChanged();
    }
}
