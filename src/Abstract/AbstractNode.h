//
// Created by artem on 19.12.2020.
//

#ifndef DBNODES_ABSTRACTNODE_H
#define DBNODES_ABSTRACTNODE_H

#include "DrawableWidget.h"
#include "QMouseEvent"
#include "QPoint"

namespace DbNodes::Abstract {

    class AbstractNode : public DrawableWidget
    {
        private:
            bool moveRestrictions = true;

        public:
            explicit AbstractNode(QWidget *parent = nullptr);

        protected:
            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;

            void enableMoveRestrictions(const bool &enable);

            QPoint oldPos{};
    };

}


#endif //DBNODES_ABSTRACTNODE_H
