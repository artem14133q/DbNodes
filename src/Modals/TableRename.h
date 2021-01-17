//
// Created by artem on 26.12.2020.
//

#ifndef DBNODES_TABLERENAME_H
#define DBNODES_TABLERENAME_H

#include "QMainWindow"

namespace DbNodes::Modals {

    class TableRename: public QMainWindow
    {
        Q_OBJECT

        public:
            explicit TableRename(QString name, QWidget* parent = nullptr);

        signals:
            void pushConfirm(const QString &name);

        public slots:
            void setTitleName(const QString &name);
            void confirm();
            void cancel();

        private:
            QString nameTitle;

            void initUi();
    };

}

#endif //DBNODES_TABLERENAME_H
