//
// Created by artem on 20.03.2021.
//

#ifndef DBNODES_SELECTABLE_H
#define DBNODES_SELECTABLE_H

#include "QObject"
#include "QPoint"

#include "AbstractUtil.h"

namespace DbNodes::Utils::MultipleSelection {

    class Selectable: public AbstractUtil
    {
        Q_OBJECT

        private:
            bool isClicked = false;
            bool isEnabled = true;

        public:
            explicit Selectable(QObject *object = nullptr);

            void setClicked(const bool &clicked);

            void move(const QPoint &delta);

            void flush();

            void setEnable(bool enable);
            bool enabled();

        public: signals:
            void moveSignal(QObject *node, const QPoint &delta);
            void unSelectNodesSignal();
            void selectCurrentNodeSignal();
            void unSelectCurrentNodeSignal();
    };

}


#endif //DBNODES_SELECTABLE_H
