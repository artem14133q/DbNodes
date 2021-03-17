//
// Created by artem on 15.03.2021.
//

#include "QHBoxLayout"
#include "QLabel"
#include "QPushButton"

#include "ExceptionModal.h"
#include "../helper.h"

namespace DbNodes::Modals {

    ExceptionModal::ExceptionModal(const QString &title, const QString &text, QWidget *parent)
        : AbstractModal(parent)
    {
        setWindowTitle(title);

        parentWidget()->setDisabled(true);
        setEnabled(true);

        setStyleSheet(Helper::getStyleFromFile("subWindow"));

        initUI(text);

        Helper::moveToCenter(parent, this);

        show();
    }

    void ExceptionModal::initUI(const QString &text)
    {
        auto *generalWidget = new QWidget(this);

        auto *vl = new QVBoxLayout();
        vl->setContentsMargins(10, 10, 10, 10);

        auto *hl = new QHBoxLayout();

        auto *errorIcon = new QLabel(this);
        errorIcon->setPixmap(QPixmap(Helper::getIconPath("cross", false)));
        hl->addWidget(errorIcon);

        hl->addStretch();

        auto *textBrowser = new QLabel(text, this);
        textBrowser->setStyleSheet("QLabel{color: #efefef; font: bold; font-size: 16px;}");
        hl->addWidget(textBrowser);

        hl->addStretch();

        vl->addLayout(hl);

        auto btl = new QHBoxLayout();
        btl->addStretch();

        auto *okButton = new QPushButton("Ok", this);
        okButton->setStyleSheet(Helper::getStyleFromFile("button"));
        connect(okButton, &QPushButton::clicked, this, &ExceptionModal::confirm);
        okButton->setFixedSize(70, 30);

        btl->addWidget(okButton);

        vl->addLayout(btl);

        generalWidget->setLayout(vl);

        setCentralWidget(generalWidget);
        adjustSize();

        if (width() < 400) {
            setFixedWidth(400);
        }
    }

    void ExceptionModal::confirm()
    {
        exit();
    }

    void ExceptionModal::exit()
    {
        parentWidget()->setDisabled(false);

        AbstractModal::exit();
    }
}