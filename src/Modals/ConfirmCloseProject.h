//
// Created by artem on 20.12.2020.
//

#ifndef DBNODES_CONFIRMCLOSEPROJECT_H
#define DBNODES_CONFIRMCLOSEPROJECT_H

#include "QMessageBox"
#include "QCloseEvent"

namespace DbNodes::Modals {

    class ConfirmCloseProject: public QMessageBox
    {
        Q_OBJECT

        public:
            enum Type {
                WithoutSave,
                CloseAndSave,
                NotClosed
            };

            explicit ConfirmCloseProject(const QString &projectName, QWidget *parent = nullptr);
            Type getProjectCloseType();

        private:
            QPushButton *pbCloseWithoutSave{};
            QPushButton *pbCloseAndSave{};
            QPushButton *cancel{};

            void initUi();

            void closeEvent(QCloseEvent *closeEvent) override;
    };

}


#endif //DBNODES_CONFIRMCLOSEPROJECT_H
