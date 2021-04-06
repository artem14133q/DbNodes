//
// Created by artem on 21.03.2021.
//

#ifndef DBNODES_MENU_H
#define DBNODES_MENU_H

#include "QVBoxLayout"

#include "DrawableWidget.h"
#include "Project.h"
#include "Files/ProjectListFileResolver.h"

namespace DbNodes::Widgets::StartupWidget {

    class Menu: public Abstract::DrawableWidget
    {
        Q_OBJECT

        private:
            Saving::ProjectListFileResolver *projectsManager;
            QHash<QString, Project *> projects;

            QVBoxLayout *projectsViewContainer{};

            void initUI();

            Project *createProjectWidget(const QString &path, const QString &title);

            void deleteProjects();

        public:
            explicit Menu(Saving::ProjectListFileResolver *projectsManager, QWidget *parent = nullptr);

        public slots:
            void deleteProject(const QString &path);
            void fillProjectsViewContainer();

        public: signals:
            void openProjectSignal(const QString &path);
            void updateMenuSignal();
    };

}

#endif //DBNODES_MENU_H
