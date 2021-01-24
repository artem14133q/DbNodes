#include "QLabel"
#include "QPushButton"

#include "MainWindow.h"
#include "NameNewProject.h"
#include "../helper.h"

namespace DbNodes::Modals {

    NameNewProject::NameNewProject(QWidget *parent): Abstract::AbstractModal(parent)
    {
        setFixedSize(300, 80);

        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(true);
        setDisabled(false);

        setWindowTitle("Create new project");
        setStyleSheet(Helper::getStyleFromFile("subWindow"));

        installEventFilter(this);

        initUi();
        show();
    }

    void NameNewProject::initUi()
    {
        QString buttonStyle = Helper::getStyleFromFile("button");

        auto *name = new QLabel("Name:", this);
        name->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        name->move(10, 5);

        nameInput = new QLineEdit(this);
        nameInput->installEventFilter(this);
        connect(nameInput, &QLineEdit::textChanged, this, &NameNewProject::setProjectName);
        nameInput->setStyleSheet(Helper::getStyleFromFile("lineEdit"));
        nameInput->setFixedWidth(200);
        nameInput->move(60, 5);

        auto *cancel = new QPushButton("Cancel", this);
        connect(cancel, &QPushButton::clicked, this, &NameNewProject::exit);
        cancel->setStyleSheet(buttonStyle);
        cancel->setFixedWidth(80);
        cancel->move(110, 45);

        auto *create = new QPushButton("Confirm", this);
        connect(create, &QPushButton::clicked, this, &NameNewProject::confirm);
        create->setStyleSheet(buttonStyle);
        create->setFixedWidth(80);
        create->move(200, 45);
    }

    void NameNewProject::confirm()
    {
        if (projectName == "") {
            nameInput->setStyleSheet(Helper::getStyleFromFile("lineEditError"));
            return;
        }

        auto *mainWindow = dynamic_cast<DbNodes::Widgets::MainWindow*>(parentWidget());
        mainWindow->createWorkArea(projectName);

        Abstract::AbstractModal::confirm();
    }

    void NameNewProject::setProjectName(const QString &name)
    {
        this->projectName = name;
    }

    void NameNewProject::exit()
    {
        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(false);

        Abstract::AbstractModal::exit();
    }
}
