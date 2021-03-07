//
// Created by artem on 20.12.2020.
//

#ifndef DBNODES_CONFIRMCLOSEPROJECT_H
#define DBNODES_CONFIRMCLOSEPROJECT_H

#define PROJECT_CLOSE_WITHOUT_SAVE 1
#define PROJECT_CLOSE_AND_SAVE 2
#define PROJECT_NOT_CLOSED 3

#include "QMessageBox"
#include "QCloseEvent"

namespace DbNodes::Modals {

    class ConfirmCloseProject: public QMessageBox
    {
        Q_OBJECT

        public:
            explicit ConfirmCloseProject(const QString &projectName, QWidget *parent = nullptr);
            int getProjectCloseType();

        private:
            QPushButton *pbCloseWithoutSave{};
            QPushButton *pbCloseAndSave{};
            QPushButton *cancel{};

            void initUi();

            void closeEvent(QCloseEvent *closeEvent) override;
    };

}


#endif //DBNODES_CONFIRMCLOSEPROJECT_H
