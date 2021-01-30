#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QScrollArea"
#include "Workarea.h"
#include "QCloseEvent"
#include "ConfirmCloseProject.h"
#include "StartupWidget.h"
#include "QSettings"

#include "config.h"

#define SAVE_TYPE_NEW_FILE 0
#define SAVE_TYPE_REWRITE_FILE 1

namespace DbNodes::Widgets {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = nullptr);
            void createWorkArea(const QString &projectName);

        public slots:
            void closeCurrentProject(const int &closeProjectStatus);
            void generateSaveFile(const int &saveType);
            void openSaveFile();
            void createNewProject();

        private:
            QAction* createProject{};
            QAction* openProject{};
            QAction* saveProject{};
            QAction* saveAsProject{};
            QAction* closeProject{};
            QAction* exit{};

            QAction* findNode{};

            QScrollArea* scrollArea;
            StartupWidget* startupWidget;
            QString filePath = "";

            WorkArea* workArea{};

            QMenuBar* defineMenuBar();
            void setTitle(const QString &name, const QString &path);
            int openConfirmCloseProjectModal();

            void closeEvent(QCloseEvent * event) override;
            void paintEvent(QPaintEvent * event) override;
    };

}
#endif // MAINWINDOW_H
