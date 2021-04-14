//
// Created by artem on 20.03.2021.
//

#ifndef DBNODES_REPOSITORY_H
#define DBNODES_REPOSITORY_H

#include "QPainter"

#include "AbstractNode.h"

namespace DbNodes::Utils::MultipleSelection {

    class Repository: public QObject
    {
        Q_OBJECT

        private:
            QList<Abstract::NodePtr> selectedNodes;

            bool mousePressed = false;
            QPoint mouseStartPos;
            QPoint mouseCurrentPos;

            QWidget *parentWidget();

        public:
            explicit Repository(QWidget *parent = nullptr);

            void unselectNodes();
            static void setSelectToNode(const Abstract::NodePtr &node, const bool &select);
            void insertNodeToSelectionList(const Abstract::NodePtr &node);
            void removeNodeFromSelectionList(const Abstract::NodePtr &node);

            void moveSelectedNode(QObject *node, const QPoint &delta);
            void initDefaultsConnections(const Abstract::NodePtr &node);

            void drawSelectionRect(QPainter &painter);

            void start(const QPoint &mousePos);
            void move(const QPoint &mousePos, const QList<Abstract::NodePtr> &nodes);
            void stop();

            void deleteSelected();

            void initDefaultActionsForUtil(QMenu *menu);

            ~Repository() override;

        public: signals:
            void deleteSelectedNodesSignal();
    };

}


#endif //DBNODES_REPOSITORY_H
