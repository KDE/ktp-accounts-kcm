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
#include <QtGui/QCheckBox>
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

    // Create all the possible widgets for displaying the parameter.
    QLabel *nameLabel = new QLabel();
    QLineEdit *lineEdit = new QLineEdit();
    QCheckBox *checkBox = new QCheckBox();

    // Connect to the slots from the widgets that we are interested in.
    connect(lineEdit, SIGNAL(textChanged(QString)), SLOT(onLineEditTextChanged(QString)));
    connect(checkBox, SIGNAL(toggled(bool)), SLOT(onCheckBoxToggled(bool)));

    widgets << nameLabel << lineEdit << checkBox;

    return widgets;
}

void ParameterEditDelegate::updateItemWidgets(const QList<QWidget*> widgets,
                                              const QStyleOptionViewItem &option,
                                              const QPersistentModelIndex &index) const
{
    int margin = option.fontMetrics.height() / 2;
    int right = option.rect.width();

    // Draw the label showing the name of the parameter
    QLabel *nameLabel = qobject_cast<QLabel*>(widgets.at(0));

    nameLabel->setText(index.model()->data(index, ParameterEditModel::NameRole).toString());
    nameLabel->move(margin, 0);
    nameLabel->resize(QSize(((right - (4 * margin)) / 2), option.rect.height()));

    // Get all the optional input widgets and hide them all so they don't all appear randomly
    // for every single item in the view.
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widgets.at(1));
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(widgets.at(2));

    lineEdit->hide();
    checkBox->hide();

    // See what type the parameter is, and draw the appropriate widget for it.
    // FIXME: Support int/uint types with appropriate validation.
    if (index.model()->data(index, ParameterEditModel::TypeRole).type() == QVariant::Bool) {
        // Bool type. Draw a checkbox.
        checkBox->move((right / 2) + margin, (option.rect.height() - checkBox->size().height()) / 2);
        checkBox->show();
        checkBox->setFocus(Qt::OtherFocusReason);
        // NB: We must update the value of the widget AFTER setting it as focused, otherwise
        // focusedItem() returns the wrong value and we end up setting the data of the wrong item
        // in the model.
        checkBox->setChecked(index.model()->data(index, ParameterEditModel::ValueRole).toBool());
    } else {
        // For any other type, treat it as a string type.
        // FIXME: Support asterisking out the entry in secret parameters
        lineEdit->move((right / 2) + margin, (option.rect.height() - lineEdit->size().height()) / 2);
        lineEdit->resize(QSize(((right - (4 * margin)) / 2), lineEdit->size().height()));
        lineEdit->show();
        lineEdit->setFocus(Qt::OtherFocusReason);
        // NB: We must update the value of the widget AFTER setting it as focused, otherwise
        // focusedItem() returns the wrong value and we end up setting the data of the wrong item
        // in the model.
        lineEdit->setText(index.model()->data(index, ParameterEditModel::ValueRole).toString());
    }
}

void ParameterEditDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    Q_UNUSED(index);

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
    Q_UNUSED(index);

    // FIXME: There must be a better way of calculating these?!?!

    QSize size;

    size.setWidth(option.fontMetrics.height() * 4);
    size.setHeight(option.fontMetrics.height() * 2);

    return size;
}

void ParameterEditDelegate::onLineEditTextChanged(QString text)
{
    kDebug();

    QModelIndex index = focusedIndex();

    Q_EMIT dataChanged(index, QVariant(text), ParameterEditModel::ValueRole);
}

void ParameterEditDelegate::onCheckBoxToggled(bool checked)
{
    kDebug();

    QModelIndex index = focusedIndex();

    Q_EMIT dataChanged(index, QVariant(checked), ParameterEditModel::ValueRole);
}


#include "parameter-edit-delegate.moc"

