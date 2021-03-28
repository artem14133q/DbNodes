//
// Created by artem on 21.03.2021.
//

#ifndef DBNODES_PROJECT_H
#define DBNODES_PROJECT_H

#include "QMenu"
#include "QToolButton"

#include "DrawableWidget.h"

namespace DbNodes::Widgets::StartupWidget {

    class Project: public Abstract::DrawableWidget
    {
        Q_OBJECT

        private:
            void initUi();

            QString projectName, path;

            bool deleted;

            QMenu *createToolMenu(QToolButton *button);

        public:
            explicit Project(
                QString projectName,
                QString path,
                const bool &deleted,
                QWidget *parent = nullptr
            );

            void mousePressEvent(QMouseEvent *event) override;

        public: signals:
            void clicked();
            void deleteItSignal(const QString &path);
    };

}

#endif //DBNODES_PROJECT_H
