#include "QMenuBar"
#include "QDesktopWidget"
#include "QFileDialog"
#include "QFile"
#include "QTextStream"
#include "QApplication"
#include "QDebug"
#include "iostream"

#include "NameNewProject.h"
#include "Settings.h"
#include "MainWindow.h"
#include "Workarea.h"
#include "Finder.h"
#include "SaveFileTypesDictionary.h"
#include "Files/DbnFileResolver.h"

#include "../helper.h"

namespace DbNodes::Widgets {

    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        // Set minimum size
        setMinimumSize(800, 600);
        setObjectName("MainWindow");
        setStyleSheet(Helper::getStyleFromFile("main"));
        setWindowIcon(QIcon(Helper::getIconPath("128", false)));
        // Insert menu bar into MainWindow
        setMenuBar(defineMenuBar());

        startupWidget = new StartupWidget(this);

        saveManager = new Saving::SaveManager(this);

        scrollArea = new QScrollArea(this);
        scrollArea->setObjectName("ScrollArea");
        scrollArea->hide();
        setCentralWidget(scrollArea);

        showMaximized();
    }

    void MainWindow::setTitle(const QString &name, const QString &path)
    {
        setWindowTitle(QString("[%1] @ %2 - DbNodes").arg(path).arg(name));
    }

    void MainWindow::createWorkArea(const QString &projectName)
    {
        // Init new work area widget
        workArea = new WorkArea(scrollArea);

        if (projectName != nullptr) {
            workArea->setProjectName(projectName);
        }
    }

    void MainWindow::closeCurrentProject(const int &closeProjectStatus)
    {
        if (closeProjectStatus == PROJECT_NOT_CLOSED) return;

        if (closeProjectStatus == PROJECT_CLOSE_AND_SAVE) {
            generateSaveFile(filePath.isEmpty() ? SAVE_TYPE_NEW_FILE : SAVE_TYPE_REWRITE_FILE);
        }

        enableWorkArea(false);

        setWindowTitle("DbNodes");
        filePath.clear();
    }

    QMenuBar* MainWindow::defineMenuBar()
    {
        // Create MenuBar for MainWindow
        auto *menuBar = new QMenuBar(this);

        // Init menu for project
        auto *project = menuBar->addMenu("Project");

        // Create new project
        createProjectAction = project->addAction("Create new Project");
//        createProject->setIcon(QIcon(":/imgs/new"));
        createProjectAction->setShortcut(QKeySequence("Ctrl+A"));

        // Open project from file
        openProjectAction = project->addAction("Open Project");
//        openProject->setIcon(QIcon(":/imgs/open"));
        openProjectAction->setShortcut(QKeySequence("Ctrl+D"));

        // Close opened project
        closeProjectAction = project->addAction("Close Project");
//        closeProject->setIcon(QIcon(":/imgs/close"));
        closeProjectAction->setDisabled(true);
        closeProjectAction->setShortcut(QKeySequence("Ctrl+W"));

        project->addSeparator();

        // Save current project in file
        saveProjectAction = project->addAction("Save Project");
//        saveProject->setIcon(QIcon(":/imgs/save"));
        saveProjectAction->setDisabled(true);
        saveProjectAction->setShortcut(QKeySequence("Ctrl+S"));

        // Save current project in other file
        saveAsProjectAction = project->addAction("Save as ...");
//        saveAsProject->setIcon(QIcon(":/imgs/save_as"));
        saveAsProjectAction->setDisabled(true);
        saveAsProjectAction->setShortcut(QKeySequence("Ctrl+Shift+S"));

        project->addSeparator();

        openSettingsAction = project->addAction("Settings");
        openSettingsAction->setShortcut(QKeySequence("Ctrl+M"));

        project->addSeparator();

        // Open project from file
        exitAction = project->addAction("Exit");
//        openProject->setIcon(QIcon(":/imgs/open"));
        exitAction->setShortcut(QKeySequence("Ctrl+Q"));

        // Define slots
        connect(createProjectAction, &QAction::triggered, this, &MainWindow::createNewProject);
        connect(openProjectAction, &QAction::triggered, this, &MainWindow::openSaveFile);
        connect(exitAction, &QAction::triggered, this, &MainWindow::close);

        connect(closeProjectAction, &QAction::triggered, this, [this] () {
            closeCurrentProject(MainWindow::openConfirmCloseProjectModal());
        });

        connect(saveProjectAction, &QAction::triggered, this, [this] () {
            generateSaveFile(SAVE_TYPE_REWRITE_FILE);
        });

        connect(saveAsProjectAction, &QAction::triggered, this, [this] () {
            generateSaveFile(SAVE_TYPE_NEW_FILE);
        });

        connect(openSettingsAction, &QAction::triggered, this, [this] () {
            auto *window = new Modals::Settings(this);

            window->move(
                x() + width() / 2 - window->width() / 2,
                y() + height() / 2 - window->height() / 2
            );
        });

        auto *tools = menuBar->addMenu("Tools");

        findNodeAction = tools->addAction("Find ...");
        findNodeAction->setShortcut(QKeySequence("Ctrl+F"));
        findNodeAction->setEnabled(false);

        connect(findNodeAction, &QAction::triggered, this, [this] () {
            auto *window = new Modals::Finder(workArea->getAllNodes(), this);

            connect(window, &Modals::Finder::selected, workArea, &WorkArea::scrollToNode);
        });

        // return QMenuBar
        return menuBar;
    }

    void MainWindow::createNewProject()
    {
        auto window = new Modals::NameNewProject(this);
        window->move(x() + width() / 2 - window->width() / 2, y() + height() / 2 - window->height() / 2);
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        if (workArea != nullptr) {
            event->ignore();

            int closeType = MainWindow::openConfirmCloseProjectModal();
            closeCurrentProject(closeType);

            if (closeType != PROJECT_NOT_CLOSED) QApplication::exit();
        }
    }

    int MainWindow::openConfirmCloseProjectModal()
    {
        using namespace DbNodes::Modals;

        ConfirmCloseProject confirmWindow(workArea->getProjectName());

        confirmWindow.exec();

        return confirmWindow.getProjectCloseType();
    }

    void MainWindow::paintEvent(QPaintEvent * event)
    {
        if (workArea == nullptr) {
            startupWidget->move(
            width() / 2 - startupWidget->width() / 2,
            height() / 2 - startupWidget->height() / 2
            );
        }

        QMainWindow::paintEvent(event);
    }

    void MainWindow::generateSaveFile(const int &saveType)
    {
        if (filePath.isEmpty() || saveType == SAVE_TYPE_NEW_FILE) {
            filePath = Saving::SaveManager::createNewFile();
        }

        Saving::DbnFileResolver(saveManager, workArea).save(filePath);
    }

    void MainWindow::openSaveFile()
    {
        createWorkArea();

        Saving::DbnFileResolver saveFile(saveManager, workArea);

        if (!saveFile.load()) {
            delete workArea;
            workArea = nullptr;
            return;
        }

        enableWorkArea(true);

        filePath = saveManager->getLastOpenFilePath();
        setTitle(saveFile.getProjectName(), filePath);
    }

    void MainWindow::enableWorkArea(const bool &enable)
    {
        if (enable) {
            scrollArea->setWidget(workArea);
            scrollArea->show();
            startupWidget->hide();
        } else {
            scrollArea->hide();
            startupWidget->show();
            delete workArea;
            workArea = nullptr;
            scrollArea->setWidget(nullptr);
        }

        saveProjectAction->setEnabled(enable);
        saveAsProjectAction->setEnabled(enable);
        closeProjectAction->setEnabled(enable);
        findNodeAction->setEnabled(enable);
    }

    void MainWindow::createProject(const QString &name)
    {
        createWorkArea(name);
        enableWorkArea(true);
        setTitle(name, "~");
    }
}
