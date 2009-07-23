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

#include <KDebug>

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
    kDebug();

    // TODO: Implement me!

    return QList<QWidget*>();
}

void ParameterEditDelegate::updateItemWidgets(const QList<QWidget*> widgets,
                                              const QStyleOptionViewItem &option,
                                              const QPersistentModelIndex &index) const
{
    kDebug();

    // TODO: Implement me!
    Q_UNUSED(widgets);
    Q_UNUSED(option);
    Q_UNUSED(index);
}

void ParameterEditDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    // TODO: Implement me!

    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(index);
}

QSize ParameterEditDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    // TODO: Implement me!

    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize();
}


#include "parameter-edit-delegate.moc"

