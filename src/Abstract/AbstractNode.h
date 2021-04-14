//
// Created by artem on 19.12.2020.
//

#ifndef DBNODES_ABSTRACTNODE_H
#define DBNODES_ABSTRACTNODE_H

#include "DrawableWidget.h"
#include "QMouseEvent"
#include "QPoint"
#include "QPointer"
#include "QMenu"

#include "MultipleSelection/Selectable.h"

namespace DbNodes::Abstract {

    class AbstractNode;

    typedef QPointer<Abstract::AbstractNode> NodePtr;

    class AbstractNode : public DrawableWidget
    {
        private:
            bool moveRestrictions = true;
            QPoint oldPos{};

            Utils::MultipleSelection::Selectable *selectable;

        public:
            explicit AbstractNode(QWidget *parent = nullptr);
            void restrictedMove(int x, int y);
            void restrictedMove(const QPoint &pos);

            NodePtr toNode();

            Utils::MultipleSelection::Selectable *getSelectionUtil();

        protected:
            QAction *deleteNodeAction{};

            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            void enableMoveRestrictions(const bool &enable);

            void createDefaultActions(QMenu *menu);
    };
}


#endif //DBNODES_ABSTRACTNODE_H
