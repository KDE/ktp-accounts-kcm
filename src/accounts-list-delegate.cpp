/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "accounts-list-delegate.h"

#include "account-item.h"
#include "accounts-list-model.h"
#include "edit-display-name-button.h"
#include "change-icon-button.h"

#include <KTp/presence.h>
#include <KTp/Models/accounts-model.h>

#include <KDE/KLocale>
#include <KDE/KIconButton>
#include <KDE/KMenu>
#include <KDE/KAction>
#include <KDE/KDebug>

#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QCheckBox>
#include <QtGui/QAbstractItemView>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QLabel>
#include <sys/stat.h>


AccountsListDelegate::AccountsListDelegate(QAbstractItemView *itemView, QObject *parent)
    : KWidgetItemDelegate(itemView, parent)
{
}

QSize AccountsListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    int iconHeight = option.decorationSize.height() + (m_vpadding*2);  //icon height + padding either side
    int textHeight = option.fontMetrics.height()*2 + (m_vpadding*2) + 10; // text height * 2 + padding + some space between the lines

    return QSize(-1,qMax(iconHeight, textHeight)); //any width,the view should give us the whole thing.
}

QList<QWidget*> AccountsListDelegate::createItemWidgets() const
{
    // Items created by this method and added to the list returned will be
    // deleted by KWidgetItemDelegate

    QCheckBox *checkbox = new QCheckBox();
    connect(checkbox, SIGNAL(clicked(bool)), SLOT(onCheckBoxToggled(bool)));

    ChangeIconButton *changeIconButton = new ChangeIconButton();
    changeIconButton->setFlat(true);
    changeIconButton->setToolTip(i18n("Change account icon"));
    changeIconButton->setWhatsThis(i18n("This button allows to change the icon for your account.<br />"
                                  "This icon is just used locally on your computer, your contacts will not be able to see it."));

    QLabel *statusTextLabel = new QLabel();
    QLabel *statusIconLabel = new QLabel();

    EditDisplayNameButton *displayNameButton = new EditDisplayNameButton();
    displayNameButton->setFlat(true);
    displayNameButton->setToolTip(i18n("Change account display name"));
    displayNameButton->setWhatsThis(i18n("This button allows to change the display name for your account.<br />"
                                         "The display name is an alias for your account and is just used locally "
                                         "on your computer, your contacts will not be able to see it."));

    QLabel *connectionErrorLabel = new QLabel();

    return QList<QWidget*>() << checkbox
                             << changeIconButton
                             << statusTextLabel
                             << statusIconLabel
                             << displayNameButton
                             << connectionErrorLabel;
}


void AccountsListDelegate::updateItemWidgets(const QList<QWidget *> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    // draws:
    //                   AccountName
    // Checkbox | Icon |              | ConnectionIcon | ConnectionState
    //                   errorMessage

    if (!index.isValid()) {
        return;
    }

    Q_ASSERT(widgets.size() == 6);

    // Get the widgets
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(widgets.at(0));
    ChangeIconButton* changeIconButton = qobject_cast<ChangeIconButton*>(widgets.at(1));
    QLabel *statusTextLabel = qobject_cast<QLabel*>(widgets.at(2));
    QLabel *statusIconLabel = qobject_cast<QLabel*>(widgets.at(3));
    EditDisplayNameButton *displayNameButton = qobject_cast<EditDisplayNameButton*>(widgets.at(4));
    QLabel *connectionErrorLabel = qobject_cast<QLabel*>(widgets.at(5));

    Q_ASSERT(checkbox);
    Q_ASSERT(changeIconButton);
    Q_ASSERT(statusTextLabel);
    Q_ASSERT(statusIconLabel);
    Q_ASSERT(displayNameButton);
    Q_ASSERT(connectionErrorLabel);


    bool isSelected(itemView()->selectionModel()->isSelected(index) && itemView()->hasFocus());
    bool isEnabled(index.data(Qt::CheckStateRole).toBool());
    KIcon accountIcon(index.data(Qt::DecorationRole).value<QIcon>());
    KIcon statusIcon(index.data(AccountsListModel::ConnectionStateIconRole).value<QIcon>());
    QString statusText(index.data(AccountsListModel::ConnectionStateDisplayRole).toString());
    QString displayName(index.data(Qt::DisplayRole).toString());
    QString connectionError(index.data(AccountsListModel::ConnectionErrorMessageDisplayRole).toString());
    Tp::AccountPtr account(index.data(AccountsListModel::AccountRole).value<Tp::AccountPtr>());


    QRect outerRect(0, 0, option.rect.width(), option.rect.height());
    QRect contentRect = outerRect.adjusted(m_hpadding,m_vpadding,-m_hpadding,-m_vpadding); //add some padding


    // checkbox
    if (isEnabled) {
        checkbox->setChecked(true);;
        checkbox->setToolTip(i18n("Disable account"));
    } else {
        checkbox->setChecked(false);
        checkbox->setToolTip(i18n("Enable account"));
    }

    int checkboxLeftMargin = contentRect.left();
    int checkboxTopMargin = (outerRect.height() - checkbox->height()) / 2;
    checkbox->move(checkboxLeftMargin, checkboxTopMargin);


    // changeIconButton
    changeIconButton->setIcon(accountIcon);
    changeIconButton->setAccount(account);
    // At the moment (KDE 4.8.1) decorationSize is not passed from KWidgetItemDelegate
    // through the QStyleOptionViewItem, therefore we leave default size unless
    // the user has a more recent version.
    if (option.decorationSize.width() > -1) {
        changeIconButton->setButtonIconSize(option.decorationSize.width());
    }

    int changeIconButtonLeftMargin = checkboxLeftMargin + checkbox->width();
    int changeIconButtonTopMargin = (outerRect.height() - changeIconButton->height()) / 2;
    changeIconButton->move(changeIconButtonLeftMargin, changeIconButtonTopMargin);


    // statusTextLabel
    QFont statusTextFont = option.font;
    QPalette statusTextLabelPalette = option.palette;
    if (isEnabled) {
        statusTextLabel->setEnabled(true);
        statusTextFont.setItalic(false);
    } else {
        statusTextLabel->setDisabled(true);
        statusTextFont.setItalic(true);
    }
    if (isSelected) {
        statusTextLabelPalette.setColor(QPalette::Text, statusTextLabelPalette.color(QPalette::Active, QPalette::HighlightedText));
    }
    statusTextLabel->setPalette(statusTextLabelPalette);
    statusTextLabel->setFont(statusTextFont);
    statusTextLabel->setText(statusText);
    statusTextLabel->setFixedSize(statusTextLabel->fontMetrics().boundingRect(statusText).width(),
                                  statusTextLabel->height());
    int statusTextLabelLeftMargin = contentRect.right() - statusTextLabel->width();
    int statusTextLabelTopMargin = (outerRect.height() - statusTextLabel->height()) / 2;
    statusTextLabel->move(statusTextLabelLeftMargin, statusTextLabelTopMargin);


    // statusIconLabel
    statusIconLabel->setPixmap(statusIcon.pixmap(KIconLoader::SizeSmall));
    statusIconLabel->setFixedSize(statusIconLabel->minimumSizeHint());
    int statusIconLabelLeftMargin = contentRect.right() - statusTextLabel->width() - statusIconLabel->width() - 6;
    int statusIconLabelTopMargin = (outerRect.height() - statusIconLabel->height()) / 2;
    statusIconLabel->move(statusIconLabelLeftMargin, statusIconLabelTopMargin);


    QRect innerRect = contentRect.adjusted(changeIconButton->geometry().right() - contentRect.left(),
                                           0,
                                           -statusTextLabel->width() - statusIconLabel->width() - 6,
                                           0); // rect containing account name and error message


    // displayNameButton
    QFont displayNameButtonFont = option.font;
    QPalette displayNameButtonPalette = option.palette;
    if (isEnabled) {
        displayNameButtonPalette.setColor(QPalette::WindowText, displayNameButtonPalette.color(QPalette::Active, QPalette::Text));
        displayNameButtonFont.setBold(true);
    } else {
        displayNameButtonFont.setItalic(true);
        // NOTE: Flat QPushButton use WindowText instead of ButtonText for button text color
        displayNameButtonPalette.setColor(QPalette::WindowText, displayNameButtonPalette.color(QPalette::Disabled, QPalette::Text));
    }
    if (isSelected) {
        // Account is selected
        displayNameButtonPalette.setColor(QPalette::WindowText, displayNameButtonPalette.color(QPalette::Active, QPalette::HighlightedText));
    }
    displayNameButton->setFont(displayNameButtonFont);
    displayNameButton->setPalette(displayNameButtonPalette);

    QString displayNameButtonText = displayNameButton->fontMetrics().elidedText(displayName,
                                                                                Qt::ElideRight,
                                                                                innerRect.width() - (m_hpadding*2));
    displayNameButton->setText(displayNameButtonText);
    displayNameButton->setFixedSize(displayNameButton->fontMetrics().boundingRect(displayNameButtonText).width() + (m_hpadding*2),
                                    displayNameButton->minimumSizeHint().height());
    displayNameButton->setAccount(account);

    int displayNameButtonLeftMargin = innerRect.left();
    int displayNameButtonTopMargin = innerRect.top();
    displayNameButton->move(displayNameButtonLeftMargin, displayNameButtonTopMargin);


    // connectionErrorLabel
    QFont connectionErrorLabelFont = option.font;
    QPalette connectionErrorLabelPalette = option.palette;
    if (isEnabled) {
        connectionErrorLabelPalette.setColor(QPalette::WindowText, connectionErrorLabelPalette.color(QPalette::Active, QPalette::Text));
    } else {
        connectionErrorLabelFont.setItalic(true);
        connectionErrorLabelPalette.setColor(QPalette::Text, connectionErrorLabelPalette.color(QPalette::Disabled, QPalette::Text));
    }
    if (isSelected) {
        // Account is selected
        connectionErrorLabelPalette.setColor(QPalette::Text, connectionErrorLabelPalette.color(QPalette::Active, QPalette::HighlightedText));
    }
    connectionErrorLabel->setFont(connectionErrorLabelFont);
    connectionErrorLabel->setPalette(connectionErrorLabelPalette);

    QString connectionErrorLabelText = connectionErrorLabel->fontMetrics().elidedText(connectionError,
                                                                                      Qt::ElideRight,
                                                                                      innerRect.width() - (m_hpadding*2));
    connectionErrorLabel->setText(connectionErrorLabelText);
    connectionErrorLabel->setFixedSize(connectionErrorLabel->fontMetrics().boundingRect(connectionErrorLabelText).width(),
                                       displayNameButton->height());

    int connectionErrorLabelLeftMargin = innerRect.left() + m_hpadding;
    int connectionErrorLabelTopMargin = contentRect.bottom() - displayNameButton->height();
    connectionErrorLabel->move(connectionErrorLabelLeftMargin, connectionErrorLabelTopMargin);
}

void AccountsListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
}


void AccountsListDelegate::onCheckBoxToggled(bool checked)
{
    QModelIndex index = focusedIndex();
    Q_EMIT itemChecked(index, checked);
}
