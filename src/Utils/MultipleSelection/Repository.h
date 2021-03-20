//
// Created by artem on 20.03.2021.
//

#ifndef DBNODES_REPOSITORY_H
#define DBNODES_REPOSITORY_H

#include "QWidget"
#include "Node.h"

namespace DbNodes::Utils::MultipleSelection {

    class Repository: public QObject
    {
        Q_OBJECT

        private:
            QList<NODE_POINTER> selectedNodes;

            bool mousePressed = false;
            QPoint mouseStartPos;
            QPoint mouseCurrentPos;

            QWidget *parentWidget();

        public:
            explicit Repository(QWidget *parent = nullptr);

            void unSelectNodes();
            static void setSelectToNode(const NODE_POINTER &node, const bool &select);
            void insertNodeToSelectionList(const NODE_POINTER &node);
            void removeNodeFromSelectionList(const NODE_POINTER &node);

            void moveSelectedNode(QObject *node, const QPoint &delta);
            void initDefaultsConnections(const NODE_POINTER &node);

            void drawSelectionRect(QPainter &painter);

            void start(const QPoint &mousePos);
            void move(const QPoint &mousePos, const QList<NODE_POINTER> &nodes);
            void stop();
    };

}


#endif //DBNODES_REPOSITORY_H
