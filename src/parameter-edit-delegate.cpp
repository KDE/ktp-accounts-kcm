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

#include "parameter-edit-delegate.h"

#include "parameter-edit-model.h"

#include <KDebug>

#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPainter>

ParameterEditDelegate::ParameterEditDelegate(QAbstractItemView *itemView, QObject *parent)
 : KWidgetItemDelegate(itemView, parent)
{
    kDebug();

    // TODO: Implement me!
}

ParameterEditDelegate::~ParameterEditDelegate()
{
    kDebug();

    // TODO: Implement me!
}


QList<QWidget*> ParameterEditDelegate::createItemWidgets() const
{
    QList<QWidget*> widgets;

    QLabel *nameLabel = new QLabel();
    QLineEdit *lineEdit = new QLineEdit();

    widgets << nameLabel << lineEdit;

    return widgets;
}

void ParameterEditDelegate::updateItemWidgets(const QList<QWidget*> widgets,
                                              const QStyleOptionViewItem &option,
                                              const QPersistentModelIndex &index) const
{
    int margin = option.fontMetrics.height() / 2;
    int right = option.rect.width();

    QLabel *nameLabel = qobject_cast<QLabel*>(widgets.at(0));
    nameLabel->setText(index.model()->data(index, ParameterEditModel::NameRole).toString());
    nameLabel->move(margin, 0);
    nameLabel->resize(QSize(((right - (4 * margin)) / 2), option.rect.height()));

    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widgets.at(1));
    lineEdit->setText(index.model()->data(index, ParameterEditModel::ValueRole).toString());
    lineEdit->move((right / 2) + margin, 0);
    lineEdit->resize(QSize(((right - (4 * margin)) / 2), option.rect.height()));
}

void ParameterEditDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    int margin = option.fontMetrics.height() / 2;

    QStyle *style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->setPen(QPen(option.palette.highlightedText().color()));
    } else {
        painter->setPen(QPen(option.palette.text().color()));
    }

    painter->restore();
}

QSize ParameterEditDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QSize size;

    size.setWidth(option.fontMetrics.height() * 4);
    size.setHeight(option.fontMetrics.height() * 2);

    return size;
}


#include "parameter-edit-delegate.moc"

