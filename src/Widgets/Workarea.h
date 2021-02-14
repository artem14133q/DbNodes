#ifndef WORKAREA_H
#define WORKAREA_H

#include "QWidget"
#include "QWidgetList"
#include "QPointer"
#include "QPushButton"

#include "Node.h"
#include "config.h"
#include "Relation.h"

namespace DbNodes::Widgets {

    class WorkArea : public QWidget
    {
        Q_OBJECT

        public:
            explicit WorkArea(QWidget *parent, const QString &projectName);

            static const int GET_FK_NODE_ROWS = 2;
            static const int GET_PK_NODE_ROWS = 1;

            void makeRelation(const QString &relationId, NODE_RAW_POINTER &pkNodeRaw, NODE_RAW_POINTER &fkNodeRaw);
            void setNodeRaw(NODE_RAW_POINTER &nodeRaw);
            void createNodeFromFile(const QString &id, const QString &name, const QPoint &pos);
            void scrollToNode(const QString &nodeId);
            NODE_POINTER findNode(const QString &nodeId);

            QString getProjectName();
            QVector<NODE_POINTER> getAllNodes();
            NODE_RAW_POINTER findNodeRow(int type, const QString &nodeRowId);

            const QList<RELATION_POINTER> &getAllRelations();

            ~WorkArea() override;

        private:
            QList<RELATION_POINTER> relations;
            QVector<NODE_RAW_POINTER> pkList, fkList;
            QVector<NODE_POINTER> nodeList;
            QString projectName;

            void contextMenuEvent(QContextMenuEvent *event) override;
            void paintEvent(QPaintEvent *event) override;

            void cleanNodeList();
            void createNode(const QPoint &pos);
            static void cleanNodeRowsList(QVector<NODE_RAW_POINTER> &list);

            bool isAntialiasing;

            #if APP_DEBUG

            void debugRelation();

            #endif
    };

}

#endif // WORKAREA_H
