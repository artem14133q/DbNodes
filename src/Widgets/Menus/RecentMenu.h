//
// Created by artem on 05.04.2021.
//

#ifndef DBNODES_RECENTMENU_H
#define DBNODES_RECENTMENU_H

#include "QMenu"
#include "Files/ProjectListFileResolver.h"

namespace DbNodes::Widgets::Menus {

    class RecentMenu: public QMenu
    {
        Q_OBJECT

        private:
            Saving::ProjectListFileResolver *resolver;

            QList<QAction *> actionList;

            void generateMenu();

            void fillMenu();

        public:
            explicit RecentMenu(Saving::ProjectListFileResolver *resolver, QWidget *parent = nullptr);

            void updateMenu();

        public: signals:
            void openRecentProject(const QString &path);
    };

}

#endif //DBNODES_RECENTMENU_H
