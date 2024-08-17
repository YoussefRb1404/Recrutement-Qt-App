#include "deletebuttondelegate.h"
#include <QPainter>
#include <QApplication>
#include <QPushButton>

DeleteButtonDelegate::DeleteButtonDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void DeleteButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 8) {  // 8 is the index of the delete button column
        QPushButton button;
        button.setText("Supprimer");

        // Draw the button
        QStyleOptionButton buttonOption;
        buttonOption.rect = option.rect;
        QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QWidget *DeleteButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 8) {  // 8 is the index of the delete button column
        QPushButton *button = new QPushButton(parent);
        button->setText("Supprimer");
        connect(button, &QPushButton::clicked, this, [this, index]() {
            emit deleteButtonClicked(index);
        });
        return button;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}
