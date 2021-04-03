//
// Created by artem on 20.12.2020.
//

#include "ConfirmCloseProject.h"
#include "QPushButton"
#include "QLabel"
#include "../helper.h"

namespace DbNodes::Modals {

    ConfirmCloseProject::ConfirmCloseProject(const QString &projectName, QWidget *parent): QMessageBox(parent)
    {
        setWindowTitle(projectName);
        setStyleSheet(Helper::getStyleFromFile("subWindow"));
        initUi();

        Helper::moveToCenter(parentWidget(), this);
    }

    void ConfirmCloseProject::initUi()
    {
        QString buttonStyle = Helper::getStyleFromFile("button");

        auto *title = new QLabel("Are you sure you want to close project?", this);
        title->setStyleSheet(Helper::getStyleFromFile("title"));
        title->setFixedWidth(280);
        title->move(50, 0);

        pbCloseWithoutSave = addButton(tr("Close without saving"), QMessageBox::AcceptRole);
        pbCloseWithoutSave->setStyleSheet(buttonStyle);
        pbCloseWithoutSave->setFixedSize(150, 30);

        cancel = addButton(tr("Cancel"), QMessageBox::AcceptRole);
        cancel->setStyleSheet(buttonStyle);
        cancel->setFixedSize(80, 30);

        pbCloseAndSave = addButton(tr("Close and save"), QMessageBox::AcceptRole);
        pbCloseAndSave->setStyleSheet(buttonStyle);
        pbCloseAndSave->setFixedSize(120, 30);
    }

    int ConfirmCloseProject::getProjectCloseType()
    {
        if (clickedButton() == pbCloseWithoutSave) {
            return PROJECT_CLOSE_WITHOUT_SAVE;
        } else if (clickedButton() == pbCloseAndSave) {
            return PROJECT_CLOSE_AND_SAVE;
        } else {
            return PROJECT_NOT_CLOSED;
        }
    }

    void ConfirmCloseProject::closeEvent(QCloseEvent *closeEvent)
    {
        closeEvent->ignore();
        accept();
    }
}