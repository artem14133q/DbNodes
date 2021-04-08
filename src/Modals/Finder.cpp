//
// Created by artem on 27.01.2021.
//

#include "QPushButton"
#include "QLineEdit"

#include "Finder.h"
#include "../helper.h"

namespace DbNodes::Modals {

    Finder::Finder(const QList<TABLE_POINTER> &nodeVector, QWidget *parent)
        : Abstract::AbstractModal(parent), nodeVector(nodeVector)
    {
        setFixedSize(400, 600);
        setWindowFlag(Qt::FramelessWindowHint);

        parentWidget()->setDisabled(true);
        setEnabled(true);

        setStyleSheet(Helper::getStyleFromFile("subWindow outline"));

        move(
            parentWidget()->x() + parentWidget()->width() / 2 - width() / 2,
            parentWidget()->y() + parentWidget()->height() / 2 - height() / 2
        );

        initUI();
        show();
    }

    void Finder::initUI()
    {
        auto *pbClose = new QPushButton("X", this);
        pbClose->setStyleSheet(Helper::getStyleFromFile("crossButton"));
        pbClose->setFixedSize(18, 18);
        pbClose->move(width() - pbClose->width() - 5, 5);
        connect(pbClose, &QPushButton::clicked, this, &Finder::exit);

        lineEdit = new QLineEdit(this);
        lineEdit->setStyleSheet(Helper::getStyleFromFile("lineEdit"));
        lineEdit->setFixedWidth(width() - 50);
        lineEdit->move(25, 20);
        lineEdit->setFocus();

        listWidget = new QListWidget(this);
        listWidget->setSpacing(3);
        listWidget->setFocusPolicy(Qt::NoFocus);
        listWidget->setContentsMargins(0, 0, 0, 0);
        listWidget->setStyleSheet(Helper::getStyleFromFile("tableList"));
        listWidget->setFixedSize(width() - 50, height() - 70);
        listWidget->move(25, 60);

        connect(lineEdit, &QLineEdit::textChanged, this, &Finder::filterNodes);

        connect(listWidget, &QListWidget::clicked, this, &Finder::confirm);
    }

    void Finder::exit()
    {
        listWidget->clear();
        filteredNodeList.clear();
        nodeVector.clear();

        parentWidget()->setEnabled(true);
        Abstract::AbstractModal::exit();
    }

    void Finder::filterNodes(const QString &filter)
    {
        filteredNodeList.clear();
        listWidget->clear();

        if (filter == "") return;

        QRegExp regFilter("\\w*" + filter + "\\w*");

        foreach (TABLE_POINTER node, nodeVector) {
            if (regFilter.indexIn(node->getTableName()) != -1) {
                filteredNodeList.insert(node->getTableId(), node);

                auto *listItem = new QListWidgetItem(listWidget);
                listItem->setText(node->getTableName());
                listItem->setData(Qt::UserRole, node->getTableId());
                listItem->setTextColor(QColor("white"));
                listWidget->setCurrentItem(listItem);
            }
        }

        if (listWidget->count() > 0) selectItemByIndex(0);
    }

    bool Finder::eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::KeyPress) {
            auto *keyEvent = dynamic_cast<QKeyEvent *>(event);

            switch (keyEvent->key()) {
                case Qt::Key_Up:
                    selectItemPrevious(); return true;
                case Qt::Key_Down:
                    selectItemNext(); return true;
            }
        }

        return AbstractModal::eventFilter(obj, event);
    }

    void Finder::selectItemNext()
    {
        int index = getSelectionItemIndex();

        if (listWidget->count() == -1) return;

        selectItemByIndex((++index > listWidget->count() - 1) ? 0 : index);
    }

    void Finder::selectItemPrevious()
    {
        int index = getSelectionItemIndex();

        if (listWidget->count() == -1) return;

        selectItemByIndex((--index < 0) ? listWidget->count() - 1 : index);
    }

    int Finder::getSelectionItemIndex()
    {
        auto selectionItems = listWidget->selectedItems();
        int index = -1;

        if (selectionItems.count() > 0) {
            index = listWidget->row(selectionItems[0]);
        }

        return index;
    }

    void Finder::selectItemByIndex(const int &index)
    {
        unselectItems();

        auto *item = listWidget->item(index);

        listWidget->setItemSelected(item, true);
        listWidget->scrollToItem(item);
    }

    void Finder::unselectItems()
    {
        auto selectionItems = listWidget->selectedItems();

        foreach (auto *item, selectionItems) {
            listWidget->setItemSelected(item, false);
        }
    }

    void Finder::confirm()
    {
        auto selectionItems = listWidget->selectedItems();

        if (selectionItems.count() > 0) {
            emit selected(selectionItems[0]->data(Qt::UserRole).toString());

            AbstractModal::confirm();
        }
    }
}