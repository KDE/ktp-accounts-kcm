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

signals:
    void itemChecked(const QModelIndex &index, bool checked);

public slots:

private:
    static const int m_paddingSize = 7;

private slots:
    void onCheckBoxToggled(bool checked);
};

#endif // ACCOUNTLISTDELEGATE_H
