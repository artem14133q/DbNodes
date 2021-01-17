//
// Created by artem on 20.12.2020.
//

#ifndef DBNODES_CONFIRMCLOSEPROJECT_H
#define DBNODES_CONFIRMCLOSEPROJECT_H

#define PROJECT_CLOSE_WITHOUT_SAVE 1
#define PROJECT_CLOSE_AND_SAVE 2
#define PROJECT_NOT_CLOSED 3

#include "QMainWindow"

namespace DbNodes::Modals {

    class ConfirmCloseProject: public QMainWindow
    {
            Q_OBJECT

        public:
            explicit ConfirmCloseProject(QWidget *parent = nullptr);

        signals:
            void pushConfirm(const int &closeProjectStatus);

        private:
            void initUi();

        private slots:
            void resolveProjectStatus(const int &status);
    };

}


#endif //DBNODES_CONFIRMCLOSEPROJECT_H
