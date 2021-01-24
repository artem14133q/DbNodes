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

    TableRename::TableRename(QString name, QWidget *parent)
        : Abstract::AbstractModal(parent), nameTitle(std::move(name))
    {
        setFixedSize(300, 80);
        setWindowTitle(QString("Rename table: %1").arg(nameTitle));
        setStyleSheet(Helper::getStyleFromFile("subWindow"));

        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(true);
        setEnabled(true);

        initUi();
        show();
    }

    void TableRename::initUi()
    {
        QString buttonStyle = Helper::getStyleFromFile("button");

        auto *name = new QLabel("Name:", this);
        name->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        name->move(10, 5);

        auto *nameInput = new QLineEdit(nameTitle, this);
        connect(nameInput, &QLineEdit::textChanged, this, &TableRename::setTitleName);
        nameInput->setStyleSheet(Helper::getStyleFromFile("lineEdit"));
        nameInput->setFixedWidth(200);
        nameInput->move(60, 5);

        auto *cancel = new QPushButton("Cancel", this);
        connect(cancel, &QPushButton::clicked, this, &TableRename::exit);
        cancel->setStyleSheet(buttonStyle);
        cancel->setFixedWidth(80);
        cancel->move(110, 45);

        auto *confirm = new QPushButton("Confirm", this);
        connect(confirm, &QPushButton::clicked, this, &TableRename::confirm);
        confirm->setStyleSheet(buttonStyle);
        confirm->setFixedWidth(80);
        confirm->move(200, 45);
    }

    void TableRename::setTitleName(const QString &name)
    {
        nameTitle = name;
    }

    void TableRename::confirm()
    {
        emit pushConfirm(nameTitle);

        Abstract::AbstractModal::confirm();
    }

    void TableRename::exit()
    {
        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(false);
        AbstractModal::exit();
    }
}