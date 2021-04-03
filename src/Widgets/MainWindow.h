#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QScrollArea"
#include "QSettings"
#include "QCloseEvent"

#include "Workarea.h"
#include "ConfirmCloseProject.h"
#include "SaveManager.h"

#include "Files/DbnFileResolver.h"
#include "Files/ProjectListFileResolver.h"

#include "StartupWidget/MainWidget.h"

#include "config.h"

namespace DbNodes::Widgets {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = nullptr);
            void createProject(const QString &name);

        public slots:
            void closeCurrentProject(const int &closeProjectStatus, const bool &closeApp = false);
            void generateSaveFile(const int &saveType);
            void createNewProject();
            void openSaveFile(const QString &path = nullptr);

        private:
            QString filePath = "";

            // For central widget
            QScrollArea* scrollArea = nullptr;
            WorkArea* workArea = nullptr;

            // Action for main menu
            QAction* createProjectAction{};
            QAction* openProjectAction{};
            QAction* saveProjectAction{};
            QAction* saveAsProjectAction{};
            QAction* closeProjectAction{};
            QAction* openSettingsAction{};
            QAction* exitAction{};
            QAction* findNodeAction{};

            // Files
            Saving::ProjectListFileResolver *projectListFileResolver;
            Saving::SaveManager *saveManager;

            QMenuBar* defineMenuBar();

            void setTitle(const QString &name, const QString &path);
            int openConfirmCloseProjectModal();

            void closeEvent(QCloseEvent * event) override;

            void enableWorkArea(const bool &enable);
            void createWorkArea(const QString &projectName = nullptr);

            void openLastOpenedFileIfExists();

            StartupWidget::MainWidget *createStartupWidget();
    };

}
#endif // MAINWINDOW_H
