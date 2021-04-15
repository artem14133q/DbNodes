#include "QMenuBar"
#include "QDesktopWidget"
#include "QApplication"
#include "iostream"

#include "NewProject.h"
#include "Settings.h"
#include "MainWindow.h"
#include "Workarea.h"
#include "Finder.h"

#include "../helper.h"

namespace DbNodes::Widgets {

    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        // Set minimum size
        setMinimumSize(800, 600);
        setObjectName("MainWindow");
        setStyleSheet(Helper::getStyleFromFile("main"));
        setWindowIcon(QIcon(Helper::getIconPath("128", false)));

        saveManager = new Saving::SaveManager(this);
        projectListFileResolver = new Saving::ProjectListFileResolver(saveManager, PROJECTS_LIST_FILE_PATH);

        setMenuBar(defineMenuBar());

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
        if (closeProjectStatus == Modals::ConfirmCloseProject::Type::NotClosed) return;

        if (closeProjectStatus == Modals::ConfirmCloseProject::Type::CloseAndSave) {
            generateSaveFile(
                filePath.isEmpty()
                    ? Dictionaries::SaveFileTypesDictionary::Type::NewFile
                    : Dictionaries::SaveFileTypesDictionary::Type::RewriteFile
            );
        }

        enableWorkArea(false);

        if (!closeApp) {
            setCentralWidget(createStartupWidget());
        }

        setWindowTitle("DbNodes");
        filePath.clear();

        recentProjectsMenu->updateMenu();
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

        recentProjectsMenu = new Menus::RecentMenu(projectListFileResolver, project);
        connect(recentProjectsMenu, &Menus::RecentMenu::openRecentProject, this, &MainWindow::openSaveFile);

        project->addMenu(recentProjectsMenu);

        // Close opened project
        closeProjectAction = project->addAction("Close Project");
//        closeProjectAction->setIcon(QIcon(":/imgs/close"));
        closeProjectAction->setDisabled(true);
        closeProjectAction->setShortcut(QKeySequence("Ctrl+W"));

        project->addSeparator();

        // Save current project in file
        saveProjectAction = project->addAction("Save Project");
//        saveProjectAction->setIcon(QIcon(":/imgs/save"));
        saveProjectAction->setDisabled(true);
        saveProjectAction->setShortcut(QKeySequence("Ctrl+S"));

        // Save current project in other file
        saveAsProjectAction = project->addAction("Save as ...");
//        saveAsProjectAction->setIcon(QIcon(":/imgs/save_as"));
        saveAsProjectAction->setDisabled(true);
        saveAsProjectAction->setShortcut(QKeySequence("Ctrl+Shift+S"));

        project->addSeparator();

        openSettingsAction = project->addAction("Settings");
        openSettingsAction->setShortcut(QKeySequence("Ctrl+M"));

        project->addSeparator();

        // Close app
        exitAction = project->addAction("Exit");
//        exitAction->setIcon(QIcon(":/imgs/open"));
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
            generateSaveFile(Dictionaries::SaveFileTypesDictionary::Type::RewriteFile);
        });

        connect(saveAsProjectAction, &QAction::triggered, this, [this] () {
            generateSaveFile(Dictionaries::SaveFileTypesDictionary::Type::NewFile);
        });

        connect(openSettingsAction, &QAction::triggered, this, [this] () {
            new Modals::Settings(this);
        });

        auto *tools = menuBar->addMenu("Tools");

        findTableAction = tools->addAction("Find ...");
        findTableAction->setShortcut(QKeySequence("Ctrl+F"));
        findTableAction->setEnabled(false);

        connect(findTableAction, &QAction::triggered, this, [this] () {
            auto *window = new Modals::Finder(workArea->getAllTables(), this);

            connect(window, &Modals::Finder::selected, workArea, &WorkArea::scrollToTable);
        });

        // return QMenuBar
        return menuBar;
    }

    void MainWindow::createNewProject()
    {
        auto window = new Modals::NewProject(this);

        connect(window, &Modals::NewProject::createProjectSignal, this,
            [this] (const Modals::NewProject::VariantsMap &settings) {
                if (!closeProjectIfExists()) return;

                createWorkArea(settings.value(
                    Dictionaries::DefaultProjectSettingsDictionary::getKey(
                        Dictionaries::DefaultProjectSettingsDictionary::Type::name
                    )
                ).toString());

                workArea->setFixedSize(
                    settings.value(
                        Dictionaries::DefaultProjectSettingsDictionary::getKey(
                            Dictionaries::DefaultProjectSettingsDictionary::Type::width
                        )
                    ).toInt(),
                    settings.value(
                        Dictionaries::DefaultProjectSettingsDictionary::getKey(
                            Dictionaries::DefaultProjectSettingsDictionary::Type::height
                        )
                    ).toInt()
                );

                enableWorkArea(true);

                setTitle(workArea->getProjectName(), "#");

                filePath = "";
            }
        );

        Helper::moveToCenter(this, window);
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        if (workArea != nullptr) {
            event->ignore();

            int closeType = MainWindow::openConfirmCloseProjectModal();
            closeCurrentProject(closeType, true);

            if (closeType != Modals::ConfirmCloseProject::Type::NotClosed) QApplication::exit();
        }
    }

    Modals::ConfirmCloseProject::Type MainWindow::openConfirmCloseProjectModal()
    {
        Modals::ConfirmCloseProject confirmWindow(workArea->getProjectName(), this);

        confirmWindow.exec();

        return confirmWindow.getProjectCloseType();
    }

    void MainWindow::generateSaveFile(const Dictionaries::SaveFileTypesDictionary::Type &saveType)
    {
        if (filePath.isEmpty() || saveType == Dictionaries::SaveFileTypesDictionary::Type::NewFile) {
            filePath = Saving::SaveManager::createNewFile();

            projectListFileResolver->appendNewProject(workArea->getProjectName(), filePath);
            projectListFileResolver->setLastOpenedPath(filePath);
        }

        projectListFileResolver->updateProjectName(filePath, workArea->getProjectName());

        setTitle(workArea->getProjectName(), filePath);

        Saving::DbnFileResolver(saveManager, workArea).save(filePath);
    }

    void MainWindow::openSaveFile(const QString &path)
    {
        if (!closeProjectIfExists()) return;

        createWorkArea();

        Saving::DbnFileResolver saveFile(saveManager, workArea);

        if (!saveFile.load(path)) {
            if (workArea != nullptr) return;

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

        recentProjectsMenu->updateMenu();
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
        findTableAction->setEnabled(enable);
    }

    void MainWindow::createProject(const QString &name)
    {
        createWorkArea(name);
        enableWorkArea(true);
        setTitle(name, "#");
    }

    void MainWindow::openLastOpenedFileIfExists()
    {
        auto lastFilePath = projectListFileResolver->getLastOpenedPath();

        if (lastFilePath != "" && Saving::SaveManager::fileExists(lastFilePath)) {
            openSaveFile(lastFilePath);
        } else {
            projectListFileResolver->setLastOpenedPath("");
            setCentralWidget(createStartupWidget());
        }

        recentProjectsMenu->updateMenu();
    }

    StartupWidget::MainWidget *MainWindow::createStartupWidget()
    {
        auto *widget = new StartupWidget::MainWidget(
            projectListFileResolver,
            this
        );

        connect(widget, &StartupWidget::MainWidget::openProjectSignal, this, &MainWindow::openSaveFile);
        connect(widget, &StartupWidget::MainWidget::updateMenuSignal, recentProjectsMenu, &Menus::RecentMenu::updateMenu);

        return widget;
    }

    bool MainWindow::closeProjectIfExists()
    {
        if (workArea != nullptr) {
            int closeType = openConfirmCloseProjectModal();

            if (closeType == Modals::ConfirmCloseProject::Type::NotClosed) return false;

            closeCurrentProject(closeType);
        }

        return true;
    }
}
