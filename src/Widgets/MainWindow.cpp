#include "QMenuBar"
#include "QDesktopWidget"
#include "QFileDialog"
#include "QApplication"
#include "iostream"

#include "NameNewProject.h"
#include "Settings.h"
#include "MainWindow.h"
#include "Workarea.h"
#include "Finder.h"
#include "SaveFileTypesDictionary.h"

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

        saveManager = new Saving::SaveManager(this);
        projectListFileResolver = new Saving::ProjectListFileResolver(saveManager, PROJECTS_LIST_FILE_PATH);

        openLastOpenedFileIfExists();

        showMaximized();
    }

    void MainWindow::setTitle(const QString &name, const QString &path)
    {
        setWindowTitle(QString("[%1] @ %2 - DbNodes").arg(path).arg(name));
    }

    void MainWindow::createWorkArea(const QString &projectName)
    {
        scrollArea = new QScrollArea(this);
        scrollArea->setObjectName("ScrollArea");
        scrollArea->hide();

        // Init new work area widget
        workArea = new WorkArea(scrollArea);
        scrollArea->setWidget(workArea);

        if (projectName != nullptr) {
            workArea->setProjectName(projectName);
        }
    }

    void MainWindow::closeCurrentProject(const int &closeProjectStatus, const bool &closeApp)
    {
        if (closeProjectStatus == PROJECT_NOT_CLOSED) return;

        if (closeProjectStatus == PROJECT_CLOSE_AND_SAVE) {
            generateSaveFile(filePath.isEmpty() ? SAVE_TYPE_NEW_FILE : SAVE_TYPE_REWRITE_FILE);
        }

        enableWorkArea(false);

        if (!closeApp) {
            setCentralWidget(createStartupWidget());
        }

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
        connect(exitAction, &QAction::triggered, this, &MainWindow::close);

        connect(openProjectAction, &QAction::triggered, this, [this] {
            MainWindow::openSaveFile();
            projectListFileResolver->setLastOpenedPath(filePath);
        });

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
            new Modals::Settings(this);
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
            closeCurrentProject(closeType, true);

            if (closeType != PROJECT_NOT_CLOSED) QApplication::exit();
        }
    }

    int MainWindow::openConfirmCloseProjectModal()
    {
        Modals::ConfirmCloseProject confirmWindow(workArea->getProjectName(), this);

        confirmWindow.exec();

        return confirmWindow.getProjectCloseType();
    }

    void MainWindow::generateSaveFile(const int &saveType)
    {
        if (filePath.isEmpty() || saveType == SAVE_TYPE_NEW_FILE) {
            filePath = Saving::SaveManager::createNewFile();

            projectListFileResolver->appendNewProject(workArea->getProjectName(), filePath);
            projectListFileResolver->setLastOpenedPath(filePath);
        }

        projectListFileResolver->updateProjectName(filePath, workArea->getProjectName());

        Saving::DbnFileResolver(saveManager, workArea).save(filePath);
    }

    void MainWindow::openSaveFile(const QString &path)
    {
        createWorkArea();

        Saving::DbnFileResolver saveFile(saveManager, workArea);

        if (!saveFile.load(path)) {
            delete workArea;
            workArea = nullptr;

            if (centralWidget()->objectName() != "StartupWidget") {
                setCentralWidget(createStartupWidget());
            }

            return;
        }

        enableWorkArea(true);

        filePath = (path == nullptr) ? saveFile.getCurrentFilePath() : path;
        auto projectTitle = saveFile.getProjectName();

        setTitle(projectTitle, filePath);

        projectListFileResolver->appendNewProject(projectTitle, filePath);
        projectListFileResolver->setLastOpenedPath(filePath);
    }

    void MainWindow::enableWorkArea(const bool &enable)
    {
        if (enable) {
            setCentralWidget(scrollArea);
            scrollArea->show();
        } else {
            delete scrollArea;
            scrollArea = nullptr;
            workArea = nullptr;
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

    void MainWindow::openLastOpenedFileIfExists()
    {
        auto lastFilePath = projectListFileResolver->getLastOpenedPath();

        if (lastFilePath != "" || Saving::SaveManager::fileExists(lastFilePath)) {
            openSaveFile(lastFilePath);
        } else {
            setCentralWidget(createStartupWidget());
        }
    }

    StartupWidget::MainWidget *MainWindow::createStartupWidget()
    {
        auto *widget = new StartupWidget::MainWidget(
            projectListFileResolver,
            this
        );

        connect(widget, &StartupWidget::MainWidget::openProjectSignal, this, &MainWindow::openSaveFile);

        return widget;
    }
}
