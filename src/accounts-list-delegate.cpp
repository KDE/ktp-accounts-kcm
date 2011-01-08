#include "accounts-list-delegate.h"

#include "accounts-list-model.h"

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

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    QString accountName = index.data(Qt::DisplayRole).toString();
    QString connectionStatusString = index.data(AccountsListModel::ConnectionStateDisplayRole).toString();

    QRect innerRect = option.rect.adjusted(4,4,-4,-4); //add some padding

    QSize decorationSize = option.decorationSize;
    QSize statusSize = style->itemTextRect(option.fontMetrics, option.rect, Qt::AlignCenter , false , connectionStatusString).size();

    QRect iconRect(innerRect.topLeft(), decorationSize);
    QRect statusRect(QPoint(innerRect.right() - statusSize.width(), innerRect.top()), innerRect.bottomRight());


    QRect textRect(iconRect.topRight() + QPoint(5,0), statusRect.bottomLeft() - QPoint(5,0));


    painter->drawPixmap(iconRect, icon.pixmap(option.decorationSize));
    painter->drawText(textRect, accountName);

    textRect.adjust(0,option.fontMetrics.height()+2,0,0);
    painter->drawText(textRect, "Connection Error");

    painter->drawText(statusRect, Qt::AlignCenter, connectionStatusString);
}


