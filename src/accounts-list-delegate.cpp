#include "accounts-list-delegate.h"

#include "accounts-list-model.h"

#include <QApplication>
#include <QPainter>
#include <QCheckBox>

#include <KDebug>

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
    connect(checkbox, SIGNAL(clicked(bool)), SLOT(onCheckBoxToggled(bool)));
    return QList<QWidget*>() << checkbox;
}


void AccountsListDelegate::updateItemWidgets(const QList<QWidget *> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(widgets.at(0));
    if (checkbox) {
        int topMargin = (option.rect.height() - checkbox->height()) / 2;
        checkbox->move(m_paddingSize, topMargin);
        checkbox->setChecked(index.data(Qt::CheckStateRole).toBool());
    }
    else {
        kDebug() << "checkbox widget pointer is null..";
    }
}

void AccountsListDelegate::onCheckBoxToggled(bool checked)
{
    QModelIndex index = focusedIndex();
    emit dataChanged(index, QVariant((checked ? Qt::Checked : Qt::Unchecked)), Qt::CheckStateRole);
}


void AccountsListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //draws Checkbox | Icon | AccountName  | ConnectionIcon | ConnectionState
    //               |      | errorMessage |                |

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

    QFont boldFont = option.font;
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->drawText(mainTextRect, Qt::AlignTop, accountName);
    painter->setFont(option.font);
    painter->drawText(mainTextRect, Qt::AlignBottom, connectionErrorString);

    painter->drawPixmap(style->itemPixmapRect(statusIconRect, Qt::AlignCenter, statusPixmap), statusPixmap);
    painter->drawText(statusTextRect, Qt::AlignCenter, connectionStatusString);
}


