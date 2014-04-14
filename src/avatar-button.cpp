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

#include <QtGui/QWidgetAction>
#include <QDataStream>

#include <KFileDialog>
#include <KMenu>
#include <KLocalizedString>
#include <KMessageBox>
#include <KImageFilePreview>

#include <TelepathyQt/Account>

#include <KDE/KDebug>
#include <KGlobalSettings>
#include <KPixmapRegionSelectorDialog>
#include <KPixmapRegionSelectorWidget>

// It has been decided by a fair dice roll that 128px is a reasonable avatar
// size limit in case the server (or Telepathy backend) does not provide
// such information
#define AVATAR_MIN_SIZE 64
#define AVATAR_MAX_SIZE 128

AvatarButton::AvatarButton(QWidget *parent)
    : QToolButton(parent)
{
    KMenu *menu = new KMenu(this);

    setPopupMode(QToolButton::InstantPopup);

    setIconSize(QSize(64,64));

    menu->addAction(KIcon(QLatin1String("document-open-folder")), i18n("Load from file..."), this, SLOT(onLoadAvatarFromFile()));
    menu->addAction(KIcon(QLatin1String("edit-clear")), i18n("Clear Avatar"), this, SLOT(onClearAvatar()));

    setMenu(menu);
}

AvatarButton::~AvatarButton()
{

}

void AvatarButton::setAvatar(const Tp::Avatar &avatar)
{
    m_avatar = avatar;

    if (! avatar.avatarData.isNull()) {
        KIcon avatarIcon;
        QPixmap avatarPixmap = QPixmap::fromImage(QImage::fromData(avatar.avatarData));
        //scale oversized avatars to fit, but don't stretch smaller avatars
        avatarIcon.addPixmap(avatarPixmap.scaled(iconSize().boundedTo(avatarPixmap.size()), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        setIcon(avatarIcon);
    } else {
        setIcon(KIcon(QLatin1String("im-user")));
    }
}

Tp::Avatar AvatarButton::avatar() const
{
    return m_avatar;
}

void AvatarButton::setAccount(const Tp::AccountPtr& account)
{
    m_account = account;
}

void AvatarButton::onLoadAvatarFromFile()
{
    QStringList mimeTypes;
    if (m_account) {
        mimeTypes = m_account->avatarRequirements().supportedMimeTypes();
    }
    if (mimeTypes.isEmpty()) {
        mimeTypes << QLatin1String("image/jpeg")
                  << QLatin1String("image/png")
                  << QLatin1String("imgae/gif");
    }

    QPointer<KFileDialog> dialog = new KFileDialog(KUrl::fromPath(KGlobalSettings::picturesPath()),
                                                   mimeTypes.join(QLatin1String(" ")), this);
    dialog->setOperationMode(KFileDialog::Opening);
    dialog->setPreviewWidget(new KImageFilePreview(dialog));
    dialog->setCaption(i18n("Please choose your avatar"));

    KUrl fileUrl;
    if (dialog->exec()) {
        if (!dialog) {
            return;
        }

        fileUrl = dialog->selectedUrl();
    }

    delete dialog;

    if (fileUrl.isEmpty()) {
        return;
    }

    const QPixmap pixmap(fileUrl.toLocalFile());

    const Tp::AvatarSpec spec = m_account->avatarRequirements();

    const int maxWidth = spec.maximumWidth() > 0 ? spec.maximumWidth() : AVATAR_MAX_SIZE;
    const int maxHeight = spec.maximumHeight() > 0 ? spec.maximumHeight() : AVATAR_MAX_SIZE;
    const int minWidth = spec.minimumWidth() > 0 ? spec.minimumWidth() : AVATAR_MIN_SIZE;
    const int minHeight = spec.minimumHeight() > 0 ? spec.minimumHeight() : AVATAR_MIN_SIZE;

    QPixmap finalPixmap;
    if (pixmap.width() > spec.maximumWidth() || pixmap.height() > spec.maximumHeight()) {
        finalPixmap = cropPixmap(pixmap, maxWidth, maxHeight, minWidth, minHeight);
    } else {
        finalPixmap = pixmap;

        if (pixmap.width() < minWidth) {
            finalPixmap = finalPixmap.scaledToWidth(minWidth, Qt::SmoothTransformation);
        }

        if (pixmap.height() < minHeight) {
            finalPixmap = finalPixmap.scaledToHeight(minHeight, Qt::SmoothTransformation);
        }
    }

    if (finalPixmap.isNull()) {
        return;
    }

    Tp::Avatar avatar;
    avatar.MIMEType = QLatin1String("image/png");
    QDataStream stream(&avatar.avatarData, QIODevice::WriteOnly);
    if (!finalPixmap.save(stream.device(), "PNG")) {
        KMessageBox::error(this, i18n("Failed to load avatar."));
        return;
    }

    setAvatar(avatar);
    Q_EMIT avatarChanged();
}

QPixmap AvatarButton::cropPixmap(const QPixmap &pixmap, int maxWidth, int maxHeight,
                                 int minWidth, int minHeight) const
{
    //if there's no image we don't need to select a region
    if (pixmap.isNull()) {
        return pixmap;
    }

    QPointer<KPixmapRegionSelectorDialog> regionDlg = new KPixmapRegionSelectorDialog();
    KPixmapRegionSelectorWidget *widget = regionDlg->pixmapRegionSelectorWidget();
    widget->setPixmap(pixmap);
    widget->setSelectionAspectRatio(maxWidth, maxHeight);

    if (regionDlg->exec()) {
        if (!regionDlg) {
            return QPixmap();
        }

        delete regionDlg;

        QImage selectedImage = widget->selectedImage();
        if (selectedImage.width() > maxWidth || selectedImage.height() > maxHeight) {
            return QPixmap::fromImage(selectedImage.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio));
        } else if (selectedImage.width() < minWidth || selectedImage.height() < minHeight) {
            return QPixmap::fromImage(selectedImage.scaled(minWidth, minHeight, Qt::KeepAspectRatio));
        } else {
            return QPixmap::fromImage(widget->selectedImage());
        }
    }

    delete regionDlg;

    return QPixmap();
}

void AvatarButton::onClearAvatar()
{
    setAvatar(Tp::Avatar());
    Q_EMIT avatarChanged();
}
