//
// Created by artem on 20.03.2021.
//

#ifndef DBNODES_SELECTABLE_H
#define DBNODES_SELECTABLE_H

#include "QObject"
#include "QPoint"

namespace DbNodes::Utils::MultipleSelection {

    class Selectable: public QObject
    {
        Q_OBJECT

        private:
            bool isClicked = false;

        public:
            explicit Selectable(QObject *object = nullptr);

            void setClicked(const bool &clicked);

            void move(const QPoint &delta);

            void disable();

        public: signals:
            void moveSignal(QObject *node, const QPoint &delta);
            void unSelectNodesSignal();
            void selectCurrentNodeSignal();
            void unSelectCurrentNodeSignal();
    };

}


#endif //DBNODES_SELECTABLE_H
