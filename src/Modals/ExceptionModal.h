//
// Created by artem on 15.03.2021.
//

#ifndef DBNODES_EXCEPTIONMODAL_H
#define DBNODES_EXCEPTIONMODAL_H

#include "AbstractModal.h"

namespace DbNodes::Modals {

    class ExceptionModal: public Abstract::AbstractModal
    {
        public:
            explicit ExceptionModal(const QString &title, const QString &text, QWidget *parent = nullptr);

        private:
            void initUI(const QString &text);

            void confirm() override;
            void exit() override;
    };

}

#endif //DBNODES_EXCEPTIONMODAL_H
