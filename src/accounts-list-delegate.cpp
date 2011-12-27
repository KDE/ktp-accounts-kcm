/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
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

#include "accounts-list-model.h"

#include <QApplication>
#include <QPainter>
#include <QCheckBox>
#include <QAbstractItemView>

#include <KDebug>
#include <KLocale>
#include <QSortFilterProxyModel>

#include "account-item.h"

AccountsListDelegate::AccountsListDelegate(QAbstractItemView *itemView, QObject *parent)
    : KWidgetItemDelegate(itemView, parent)
{
}

QSize AccountsListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    int iconHeight = option.decorationSize.height() + (m_paddingSize*2);  //icon height + padding either side
    int textHeight = option.fontMetrics.height()*2 + (m_paddingSize*2);

    return QSize(1,qMax(iconHeight, textHeight)); //any width,the view should give us the whole thing.
}

QList<QWidget*> AccountsListDelegate::createItemWidgets() const
{
    QCheckBox *checkbox = new QCheckBox();
    checkbox->setToolTip(i18n("Enable account"));
    connect(checkbox, SIGNAL(clicked(bool)), SLOT(onCheckBoxToggled(bool)));
    return QList<QWidget*>() << checkbox;
}


void AccountsListDelegate::updateItemWidgets(const QList<QWidget *> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    QCheckBox* checkbox = qobject_cast<QCheckBox*>(widgets.at(0));
    if (checkbox) {
        int topMargin = (option.rect.height() - checkbox->height()) / 2;
        checkbox->move(m_paddingSize, topMargin);
        checkbox->setChecked(index.data(Qt::CheckStateRole).toBool());

        if(checkbox->isChecked()) {
            checkbox->setToolTip(i18n("Disable account"));
        }
        else {
            checkbox->setToolTip(i18n("Enable account"));
        }
    }
    else {
        kDebug() << "checkbox widget pointer is null..";
    }
}

void AccountsListDelegate::onCheckBoxToggled(bool checked)
{
    QModelIndex index = focusedIndex();
    emit itemChecked(index, checked);
}


void AccountsListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //draws Checkbox | Icon | AccountName  | ConnectionIcon | ConnectionState
    //               |      | errorMessage |                |

    if (!index.isValid()) {
        return;
    }

    QStyle *style = QApplication::style();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    QString accountName = index.data(Qt::DisplayRole).toString();
    QString connectionStatusString = index.data(AccountsListModel::ConnectionStateDisplayRole).toString();
    QString connectionErrorString = index.data(AccountsListModel::ConnectionErrorMessageDisplayRole).toString();
    QIcon connectionStatusIcon = index.data(AccountsListModel::ConnectionStateIconRole).value<QIcon>();

    QRect innerRect = option.rect.adjusted(m_paddingSize,m_paddingSize,-m_paddingSize,-m_paddingSize); //add some padding

    QSize checkBoxSize(32,20);
    QSize decorationSize = option.decorationSize;
    QSize statusTextSize = style->itemTextRect(option.fontMetrics, option.rect, Qt::AlignCenter , false , connectionStatusString).size();
    QSize statusIconSize(16,16); //a nice small icon

    QRect checkBoxRect(0, innerRect.top(), checkBoxSize.width(), innerRect.height());
    QRect decorationRect(checkBoxRect.right(), innerRect.top(), decorationSize.width(), innerRect.height());

    QRect statusTextRect(innerRect.right() - statusTextSize.width(), innerRect.top(), statusTextSize.width(), innerRect.height());
    QRect statusIconRect(statusTextRect.left() - statusIconSize.width() -2, innerRect.top(), statusIconSize.width(), innerRect.height());
    QRect mainTextRect(decorationRect.topRight() + QPoint(m_paddingSize,0), statusIconRect.bottomLeft());

    QPixmap iconPixmap = icon.pixmap(decorationSize);
    QPixmap statusPixmap = connectionStatusIcon.pixmap(statusIconSize);

    painter->drawPixmap(style->itemPixmapRect(decorationRect, Qt::AlignCenter, iconPixmap), iconPixmap);

    QFont accountNameFont = option.font;
    QFont statusTextFont = option.font;

    if (index.data(Qt::CheckStateRole).toBool()) {
        accountNameFont.setBold(true);
        painter->setPen(option.palette.color(QPalette::Active, QPalette::Text));
    } else {
        accountNameFont.setItalic(true);
        statusTextFont.setItalic(true);
        painter->setPen(option.palette.color(QPalette::Disabled, QPalette::Text));
    }

    if (itemView()->selectionModel()->isSelected(index) && itemView()->hasFocus()) {
        painter->setPen(option.palette.color(QPalette::Active, QPalette::HighlightedText));
    }

    // This text gets painted with the mainTextFont when the account is disabled, otherwise the default font is used
    painter->setFont(statusTextFont);
    painter->drawText(mainTextRect, Qt::AlignBottom, connectionErrorString);
    painter->drawPixmap(style->itemPixmapRect(statusIconRect, Qt::AlignCenter, statusPixmap), statusPixmap);
    painter->drawText(statusTextRect, Qt::AlignCenter, connectionStatusString);

    painter->setFont(accountNameFont);
    painter->drawText(mainTextRect, Qt::AlignTop, accountName);
}
