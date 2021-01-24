//
// Created by artem on 26.12.2020.
//

#ifndef DBNODES_TABLERENAME_H
#define DBNODES_TABLERENAME_H

#include "AbstractModal.h"

namespace DbNodes::Modals {

    class TableRename: public Abstract::AbstractModal
    {
        Q_OBJECT

        public:
            explicit TableRename(QString name, QWidget* parent = nullptr);

        signals:
            void pushConfirm(const QString &name);

        public slots:
            void setTitleName(const QString &name);
            void confirm() override;
            void exit() override;

        private:
            QString nameTitle;

            void initUi();
    };

}

#endif //DBNODES_TABLERENAME_H
