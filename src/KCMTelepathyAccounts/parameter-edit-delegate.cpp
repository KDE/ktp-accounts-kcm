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
#include <QtGui/QSpinBox>
#include <QtGui/QPainter>

ParameterEditDelegate::ParameterEditDelegate(QAbstractItemView *itemView, QObject *parent)
 : KWidgetItemDelegate(itemView, parent)
{
    kDebug();
}

ParameterEditDelegate::~ParameterEditDelegate()
{
    kDebug();
}


QList<QWidget*> ParameterEditDelegate::createItemWidgets() const
{
    QList<QWidget*> widgets;

    // Create all the possible widgets for displaying the parameter.
    QLabel *nameLabel = new QLabel();
    QLineEdit *lineEdit = new QLineEdit();
    QCheckBox *checkBox = new QCheckBox();
    QSpinBox *spinBox = new QSpinBox();

    // sets the maximum value. the minimum will be set according to the item type
    spinBox->setMaximum(2147483647);

    // Connect to the slots from the widgets that we are interested in.
    connect(lineEdit, SIGNAL(textChanged(QString)), SLOT(onLineEditTextChanged(QString)));
    connect(checkBox, SIGNAL(toggled(bool)), SLOT(onCheckBoxToggled(bool)));
    connect(spinBox, SIGNAL(valueChanged(int)), SLOT(onSpinBoxValueChanged(int)));

    widgets << nameLabel << lineEdit << checkBox << spinBox;

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

    nameLabel->setText(index.model()->data(index, ParameterEditModel::LocalizedNameRole).toString());
    nameLabel->move(margin, 0);
    nameLabel->resize(QSize(((right - (4 * margin)) / 2), option.rect.height()));

    // Get all the optional input widgets.
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widgets.at(1));
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(widgets.at(2));
    QSpinBox  *spinBox  = qobject_cast<QSpinBox*>(widgets.at(3));

    // See what type the parameter is, and draw the appropriate widget for it.
    // FIXME: Support uint types with appropriate validation.
    QVariant::Type type = index.data(ParameterEditModel::TypeRole).type();

    if (type == QVariant::Bool) {
        // Bool type. Draw a checkbox.
        checkBox->move((right / 2) + margin, (option.rect.height() - checkBox->size().height()) / 2);

        checkBox->setFocus(Qt::OtherFocusReason);
        // NB: We must update the value of the widget AFTER setting it as focused, otherwise
        // focusedItem() returns the wrong value and we end up setting the data of the wrong item
        // in the model.
        checkBox->setChecked(index.model()->data(index, ParameterEditModel::ValueRole).toBool());

        // Hide all the other input widgets for this item. This must be done in each condition
        // to avoid them losing focus (and cursor position) when updating the content of them.
        lineEdit->hide();
        spinBox->hide();

    } else if (type == QVariant::Int || type == QVariant::UInt) {
        // Integer type. Draw a spinBox
        spinBox->move((right / 2) + margin, (option.rect.height() - spinBox->size().height()) / 2);
        spinBox->resize(QSize(((right - (4 * margin)) / 2), spinBox->size().height()));

        // set the minimum value of the spinbox depending on whether it is signed or not
        if (type == QVariant::UInt)
            spinBox->setMinimum(0);
        else
            spinBox->setMinimum(-2147483648);

        spinBox->setFocus(Qt::OtherFocusReason);
        // NB: We must update the value of the widget AFTER setting it as focused, otherwise
        // focusedItem() returns the wrong value and we end up setting the data of the wrong item
        // in the model.
        spinBox->setValue(index.data(ParameterEditModel::ValueRole).toString().toInt());

        // Hide all the other input widgets for this item. This must be done in each condition
        // to avoid them losing focus (and cursor position) when updating the content of them.
        lineEdit->hide();
        checkBox->hide();

    } else {
        // For any other type, treat it as a string type.
        // FIXME: Support asterisking out the entry in secret parameters
        lineEdit->move((right / 2) + margin, (option.rect.height() - lineEdit->size().height()) / 2);
        lineEdit->resize(QSize(((right - (4 * margin)) / 2), lineEdit->size().height()));

        // If the parameter is secret, we should replace the contents with asterisks
        if (index.model()->data(index, ParameterEditModel::SecretRole).toBool()) {
            lineEdit->setEchoMode(QLineEdit::Password);
        }

        // Save the cursor position within the widget so we can restore it after altering the data
        int cursorPosition = lineEdit->cursorPosition();

        lineEdit->setFocus(Qt::OtherFocusReason);
        // NB: We must update the value of the widget AFTER setting it as focused, otherwise
        // focusedItem() returns the wrong value and we end up setting the data of the wrong item
        // in the model.
        lineEdit->setText(index.model()->data(index, ParameterEditModel::ValueRole).toString());

        // Restore the cursor position now the data has been changed.
        lineEdit->setCursorPosition(cursorPosition);

        // Hide all the other input widgets for this item. This must be done in each condition
        // to avoid them losing focus (and cursor position) when updating the content of them.
        checkBox->hide();
        spinBox->hide();
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

void ParameterEditDelegate::onSpinBoxValueChanged(int value)
{
    QSpinBox *widget = qobject_cast<QSpinBox*>(sender());

    if (!widget) {
        kWarning() << "Slot called by object of the wrong type.";
        return;
    }

    QModelIndex index = focusedIndex();

    Q_EMIT dataChanged(index, QVariant(QString::number(value)), ParameterEditModel::ValueRole);
}


#include "parameter-edit-delegate.moc"

