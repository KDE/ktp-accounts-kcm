#include "accounts-list-delegate.h"

#include <QApplication>
#include <QPainter>

AccountsListDelegate::AccountsListDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

QSize AccountsListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    int iconHeight = option.decorationSize.height() + (4*2);

    return QSize(iconHeight,50); //any random width,the view should give us the whole thing.
}

void AccountsListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyle *style = QApplication::style();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

    QRect innerRect = option.rect.adjusted(4,4,-4,-4); //add some padding
    QRect iconRect(innerRect.topLeft(), option.decorationSize);

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    painter->drawPixmap(iconRect, icon.pixmap(option.decorationSize));

    QRect textRect(iconRect.topRight(), innerRect.bottomRight());

    painter->drawText(textRect, index.data(Qt::DisplayRole).toString());
}


