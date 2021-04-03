//
// Created by artem on 24.03.2021.
//

#ifndef DBNODES_MAINWIDGET_H
#define DBNODES_MAINWIDGET_H

#include "DrawableWidget.h"
#include "Menu.h"

namespace DbNodes::Widgets::StartupWidget {

    class MainWidget: public Abstract::DrawableWidget
    {
        Q_OBJECT

        private:
            void initUI(Saving::ProjectListFileResolver *projectsManager);

        public:
            explicit MainWidget(Saving::ProjectListFileResolver *projectsManager, QWidget *parent = nullptr);

        public: signals:
            void openProjectSignal(const QString &path);
    };

}

#endif //DBNODES_MAINWIDGET_H
