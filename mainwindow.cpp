#include "mainwindow.h"
#include "qscrollarea.h"
#include "qmenubar.h"
#include "qsizepolicy.h"
#include "qdesktopwidget.h"
#include "iostream"
#include "workarea.h"
#include "qscrollarea.h"
#include "helper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set minimum size
    this->setMinimumSize(800, 600);
    this->showMaximized();
    this->setStyleSheet(Helper::getStyleFromFile("scrollArea"));
    // Insert menu bar into MainWindow
    this->setMenuBar(defineMenuBar());

    // Create scroll area for work area
    QScrollArea* scrollArea = new QScrollArea(this);

    // Init new work area widget
    this->workArea = new WorkArea(scrollArea);
    scrollArea->setWidget(workArea);
    this->setCentralWidget(scrollArea);
}

QMenuBar* MainWindow::defineMenuBar()
{
    // Create MenuBar for MainWindow
    QMenuBar* menuBar = new QMenuBar(this);

    // Init menu for project
    QMenu* project = menuBar->addMenu("Project");

    // Create new project
    QAction* createProject = project->addAction("Create new Project");
    createProject->setIcon(QIcon(QString(":/imgs/new")));
    createProject->setShortcut(QKeySequence("Ctrl+A"));

    // Open project from file
    QAction* openProject = project->addAction("Open Project");
    openProject->setIcon(QIcon(QString(":/imgs/open")));
    openProject->setShortcut(QKeySequence("Ctrl+D"));

    // Save current project in file
    QAction* saveProject = project->addAction("Save Project");
    saveProject->setIcon(QIcon(QString(":/imgs/save")));
    saveProject->setShortcut(QKeySequence("Ctrl+S"));

    // Save current project in other file
    QAction* saveAsProject = project->addAction("Save as ...");
    saveAsProject->setIcon(QIcon(QString(":/imgs/save_as")));
    saveAsProject->setShortcut(QKeySequence("Ctrl+Shift+S"));

    project->addSeparator();

    // Define slots
    connect(createProject, &QAction::triggered, this, &MainWindow::test);
    connect(saveProject, &QAction::triggered, this, &MainWindow::test);
    connect(openProject, &QAction::triggered, this, &MainWindow::test);
    connect(saveAsProject, &QAction::triggered, this, &MainWindow::test);

    // return QMenuBar
    return menuBar;
}

void MainWindow::test()
{
    std::cout << "Test" << std::endl;
}
