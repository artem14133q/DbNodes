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
        private:
            bool acceptOnEnter = false;

        public:
            void setAcceptOnEnter(const bool &enable);
            explicit AbstractModal(QWidget *parent = nullptr);
            virtual void exit();
            virtual void confirm();
            virtual void accept(const bool &close);
            bool eventFilter(QObject *obj, QEvent *event) override;
            void closeEvent(QCloseEvent *event) override;
    };

}

#endif //DBNODES_ABSTRACTMODAL_H
