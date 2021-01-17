#include "QPainter"
#include "QStyleOption"
#include "QLineEdit"
#include "QLabel"
#include "QPushButton"

#include "MainWindow.h"
#include "NameNewProject.h"
#include "../helper.h"

namespace DbNodes::Modals {

    NameNewProject::NameNewProject(QWidget *parent)
            : QMainWindow(parent)
    {
        setFixedSize(300, 120);
        setWindowTitle("Create new project");
        setWindowFlag(Qt::FramelessWindowHint);
        setStyleSheet(Helper::getStyleFromFile("subWindow"));
        initUi();
        show();
    }

    void NameNewProject::initUi()
    {
        QString buttonStyle = Helper::getStyleFromFile("button");

        auto *title = new QLabel("Create new project", this);
        title->setStyleSheet(Helper::getStyleFromFile("title"));
        title->setFixedWidth(150);
        title->move(75, 5);

        auto *name = new QLabel("Name:", this);
        name->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        name->move(10, 40);

        nameInput = new QLineEdit(this);
        nameInput->installEventFilter(this);
        connect(nameInput, &QLineEdit::textChanged, this, &NameNewProject::setProjectName);
        nameInput->setStyleSheet(Helper::getStyleFromFile("lineEdit"));
        nameInput->setFixedWidth(200);
        nameInput->move(60, 40);

        auto *cancel = new QPushButton("Cancel", this);
        connect(cancel, &QPushButton::clicked, this, &NameNewProject::cancel);
        cancel->setStyleSheet(buttonStyle);
        cancel->setFixedWidth(80);
        cancel->move(110, 80);

        auto *create = new QPushButton("Confirm", this);
        connect(create, &QPushButton::clicked, this, &NameNewProject::confirm);
        create->setStyleSheet(buttonStyle);
        create->setFixedWidth(80);
        create->move(200, 80);
    }

    void NameNewProject::confirm()
    {
        if (projectName == "") {
            nameInput->setStyleSheet(Helper::getStyleFromFile("lineEditError"));
            return;
        }

        auto *mainWindow = dynamic_cast<DbNodes::Widgets::MainWindow*>(parentWidget());
        mainWindow->createWorkArea(projectName);
        this->cancel();
    }

    void NameNewProject::cancel()
    {
        this->~NameNewProject();
    }

    void NameNewProject::setProjectName(const QString &name)
    {
        this->projectName = name;
    }

    void NameNewProject::keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Escape)
            cancel();
    }

    bool NameNewProject::eventFilter(QObject *watched, QEvent *event)
    {
        if (watched == nameInput) {
            if (event->type() == QEvent::KeyPress) {
                auto *e = dynamic_cast<QKeyEvent *>(event);
                if (e->key() == Qt::Key_Return) {
                    confirm();
                    return true;
                }
            }
        }

        return QMainWindow::eventFilter(watched, event);
    }

}
