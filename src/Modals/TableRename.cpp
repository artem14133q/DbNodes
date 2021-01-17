//
// Created by artem on 26.12.2020.
//

#include "TableRename.h"
#include "QString"
#include "QLabel"
#include "QPushButton"
#include "QLineEdit"
#include "../helper.h"

namespace DbNodes::Modals {

    TableRename::TableRename(QString name, QWidget *parent): QMainWindow(parent), nameTitle(std::move(name))
    {
        setFixedSize(300, 120);
        setWindowTitle(QString("Rename table: %1").arg(nameTitle));
        setWindowFlag(Qt::FramelessWindowHint);
        setStyleSheet(Helper::getStyleFromFile("subWindow"));

        parentWidget()->setEnabled(false);
        setEnabled(true);

        initUi();
        show();
    }

    void TableRename::initUi()
    {
        QString buttonStyle = Helper::getStyleFromFile("button");

        auto *title = new QLabel(QString("Rename table: %1").arg(nameTitle), this);
        title->setStyleSheet(Helper::getStyleFromFile("title"));
        title->adjustSize();
        title->move(width() / 2 - title->width() / 2, 5);

        auto *name = new QLabel("Name:", this);
        name->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        name->move(10, 40);

        auto *nameInput = new QLineEdit(nameTitle, this);
        connect(nameInput, &QLineEdit::textChanged, this, &TableRename::setTitleName);
        nameInput->setStyleSheet(Helper::getStyleFromFile("lineEdit"));
        nameInput->setFixedWidth(200);
        nameInput->move(60, 40);

        auto *cancel = new QPushButton("Cancel", this);
        connect(cancel, &QPushButton::clicked, this, &TableRename::cancel);
        cancel->setStyleSheet(buttonStyle);
        cancel->setFixedWidth(80);
        cancel->move(110, 80);

        auto *confirm = new QPushButton("Confirm", this);
        connect(confirm, &QPushButton::clicked, this, &TableRename::confirm);
        confirm->setStyleSheet(buttonStyle);
        confirm->setFixedWidth(80);
        confirm->move(200, 80);
    }

    void TableRename::setTitleName(const QString &name)
    {
        nameTitle = name;
    }

    void TableRename::cancel()
    {
        parentWidget()->setEnabled(true);
        this->~TableRename();
    }

    void TableRename::confirm()
    {
        emit pushConfirm(nameTitle);
        cancel();
    }
}