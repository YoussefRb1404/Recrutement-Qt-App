#ifndef DELETEBUTTONDELEGATE_H
#define DELETEBUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>

class DeleteButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit DeleteButtonDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void deleteButtonClicked(const QModelIndex &index) const;
};

#endif // DELETEBUTTONDELEGATE_H
