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

#ifndef ACCOUNTLISTDELEGATE_H
#define ACCOUNTLISTDELEGATE_H

#include <KWidgetItemDelegate>

class QCheckBox;

class AccountsListDelegate : public KWidgetItemDelegate
{
    Q_OBJECT
public:
    explicit AccountsListDelegate(QAbstractItemView *itemView, QObject *parent);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QList<QWidget*> createItemWidgets() const;
    void updateItemWidgets(const QList<QWidget *> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const;

Q_SIGNALS:
    void itemChecked(const QModelIndex &index, bool checked);

private Q_SLOTS:
    void onCheckBoxToggled(bool checked);

private:
    static const int m_hpadding = 7;
    static const int m_vpadding = 2;
};

#endif // ACCOUNTLISTDELEGATE_H
