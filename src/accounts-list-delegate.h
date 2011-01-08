#ifndef ACCOUNTLISTDELEGATE_H
#define ACCOUNTLISTDELEGATE_H

#include <QAbstractItemDelegate>

class AccountsListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit AccountsListDelegate(QObject *parent = 0);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // ACCOUNTLISTDELEGATE_H
