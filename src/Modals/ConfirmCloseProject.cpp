//
// Created by artem on 20.12.2020.
//

#include "ConfirmCloseProject.h"
#include "QPushButton"
#include "QLabel"
#include "../helper.h"

namespace DbNodes::Modals {

    ConfirmCloseProject::ConfirmCloseProject(QWidget *parent): QMainWindow(parent)
    {
        setFixedSize(400, 100);
        setWindowTitle("Are you sure you want to close project?");
        setWindowFlag(Qt::FramelessWindowHint);
        parentWidget()->setEnabled(false);
        setEnabled(true);
        setStyleSheet(Helper::getStyleFromFile("subWindow"));
        initUi();
        show();
    }

    void ConfirmCloseProject::initUi()
    {
        QString buttonStyle = Helper::getStyleFromFile("button");

        auto *title = new QLabel("Are you sure you want to close project?", this);
        title->setStyleSheet(Helper::getStyleFromFile("title"));
        title->setFixedWidth(280);
        title->move(width() / 2 - title->width() / 2, 5);

        auto *closeAndNotSave = new QPushButton("Close without saving", this);
        closeAndNotSave->setStyleSheet(buttonStyle);
        closeAndNotSave->setFixedWidth(150);
        closeAndNotSave->move(15, 60);

        auto *cancel = new QPushButton("Cancel", this);
        cancel->setStyleSheet(buttonStyle);
        cancel->setFixedWidth(80);
        cancel->move(175, 60);

        auto *closeAndSave = new QPushButton("Close and save", this);
        closeAndSave->setStyleSheet(buttonStyle);
        closeAndSave->setFixedWidth(120);
        closeAndSave->move(265, 60);

        connect(closeAndNotSave, &QPushButton::clicked, this, [this] () {
            resolveProjectStatus(PROJECT_CLOSE_WITHOUT_SAVE);
        });

        connect(cancel, &QPushButton::clicked, this, [this] () {
            resolveProjectStatus(PROJECT_NOT_CLOSED);
        });

        connect(closeAndSave, &QPushButton::clicked, this, [this] () {
            resolveProjectStatus(PROJECT_CLOSE_AND_SAVE);
        });
    }

    void ConfirmCloseProject::resolveProjectStatus(const int &status)
    {
        parentWidget()->setEnabled(true);
        emit pushConfirm(status);
        this->~ConfirmCloseProject();
    }
}