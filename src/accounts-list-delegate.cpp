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

    return QSize(iconHeight,50); //any width,the view should give us the whole thing.
}

void AccountsListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //draws Icon AccountName  ConnectionIcon ConnectionState

    QStyle *style = QApplication::style();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    QString accountName = index.data(Qt::DisplayRole).toString();
    QString connectionStatusString = index.data(AccountsListModel::ConnectionStateDisplayRole).toString();
    QString connectionErrorString = index.data(AccountsListModel::ConnectionErrorMessageDisplayRole).toString();
    QIcon connectionStatusIcon = index.data(AccountsListModel::ConnectionStateIconRole).value<QIcon>();


    QRect innerRect = option.rect.adjusted(4,4,-4,-4); //add some padding

    int top = innerRect.top();
    int bottom = innerRect.bottom();

    QSize decorationSize = option.decorationSize;
    QSize statusTextSize = style->itemTextRect(option.fontMetrics, option.rect, Qt::AlignCenter , false , connectionStatusString).size();
    QSize statusIconSize = QSize(16,16); //a nice small icon
    QSize statusSize = statusTextSize+statusIconSize;
    //the main account label size = whatever's left.

    QRect iconRect(innerRect.topLeft(), decorationSize);
//    QRect statusTextRect(QPoint(top, innerRect.right() - statusTextSize.width()), QPoint(bottom, innerRect.right()));


    QRect statusTextRect(QPoint(innerRect.right() - statusSize.width(), innerRect.top()), innerRect.bottomRight());
    int statusIconTopPadding = (innerRect.height() - statusIconSize.height()) / 2 ;
    QRect statusIconRect(QPoint(statusTextRect.left() - statusIconSize.width(), top + statusIconTopPadding), statusIconSize);

    QRect textRect(iconRect.topRight() + QPoint(10,0), QPoint(innerRect.right() - statusIconSize.width() - statusTextSize.width(), bottom));

    QFont boldFont = option.font;
    boldFont.setBold(true);

    painter->drawPixmap(iconRect, icon.pixmap(option.decorationSize));
    painter->drawPixmap(statusIconRect,connectionStatusIcon.pixmap(16,16));


    painter->setFont(boldFont);
    painter->drawText(textRect, accountName);
    painter->setFont(option.font);

    textRect.adjust(0,option.fontMetrics.height()+2,0,0);
    painter->drawText(textRect, connectionErrorString);

    painter->drawText(statusTextRect, Qt::AlignCenter, connectionStatusString);
}


