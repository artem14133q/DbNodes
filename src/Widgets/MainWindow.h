#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QScrollArea"
#include "QSettings"
#include "QCloseEvent"

#include "Workarea.h"
#include "ConfirmCloseProject.h"
#include "StartupWidget.h"
#include "SaveManager.h"

#include "config.h"

namespace DbNodes::Widgets {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = nullptr);
            void createProject(const QString &name);

        public slots:
            void closeCurrentProject(const int &closeProjectStatus);
            void generateSaveFile(const int &saveType);
            void openSaveFile();
            void createNewProject();

        private:
            QAction* createProjectAction{};
            QAction* openProjectAction{};
            QAction* saveProjectAction{};
            QAction* saveAsProjectAction{};
            QAction* closeProjectAction{};
            QAction* openSettingsAction{};
            QAction* exitAction{};

            QAction* findNodeAction{};

            QScrollArea* scrollArea;
            StartupWidget* startupWidget;
            QString filePath = "";

            WorkArea* workArea{};
            Saving::SaveManager *saveManager;

            QMenuBar* defineMenuBar();
            void setTitle(const QString &name, const QString &path);
            int openConfirmCloseProjectModal();

            void closeEvent(QCloseEvent * event) override;
            void paintEvent(QPaintEvent * event) override;

            void enableWorkArea(const bool &enable);
            void createWorkArea(const QString &projectName = nullptr);
    };

}
#endif // MAINWINDOW_H
