//
// Created by artem on 21.01.2021.
//

#ifndef DBNODES_ABSTRACTMODAL_H
#define DBNODES_ABSTRACTMODAL_H

#include "QMainWindow"
#include "QEvent"

namespace DbNodes::Abstract {

    class AbstractModal : public QMainWindow
    {
        public:
            explicit AbstractModal(QWidget *parent = nullptr);
            virtual void exit();
            virtual void confirm();
            bool eventFilter(QObject *obj, QEvent *event) override;
    };

}

#endif //DBNODES_ABSTRACTMODAL_H
