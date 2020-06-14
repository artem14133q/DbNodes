#include "namenewproject.h"
#include "qpainter.h"
#include "qstyleoption.h"
#include "qlineedit.h"
#include "qlabel.h"
#include "helper.h"
#include "qpushbutton.h"
#include "mainwindow.h"

NameNewProject::NameNewProject(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(300, 120);
    this->setWindowTitle("Create new project");
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet(Helper::getStyleFromFile("subWindow"));
    this->initUi();
    this->show();
}

void NameNewProject::initUi()
{
    QString buttonStyle = Helper::getStyleFromFile("button");

    QLabel *title = new QLabel("Create new project", this);
    title->setStyleSheet(Helper::getStyleFromFile("title"));
    title->setFixedWidth(150);
    title->move(75, 5);

    QLabel *name = new QLabel("Name:", this);
    name->move(10, 40);

    this->nameInput = new QLineEdit(this);
    connect(this->nameInput, &QLineEdit::textChanged, this, &NameNewProject::setProjectName);
    this->nameInput->setStyleSheet(Helper::getStyleFromFile("lineEdit"));
    this->nameInput->setFixedWidth(200);
    this->nameInput->move(60, 40);

    QPushButton *cancel = new QPushButton("Cancel", this);
    connect(cancel, &QPushButton::clicked, this, &NameNewProject::cancel);
    cancel->setStyleSheet(buttonStyle);
    cancel->setFixedWidth(60);
    cancel->move(150, 80);

    QPushButton *create = new QPushButton("Confirm", this);
    connect(create, &QPushButton::clicked, this, &NameNewProject::confirm);
    create->setStyleSheet(buttonStyle);
    create->setFixedWidth(60);
    create->move(220, 80);
}

void NameNewProject::confirm()
{
    if (this->projectName == "") {
        this->nameInput->setStyleSheet(Helper::getStyleFromFile("lineEditError"));
        return;
    }

    MainWindow *mainWindow = static_cast<MainWindow*>(this->parentWidget());
    mainWindow->createWorkArea(this->projectName);
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

void NameNewProject::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }
