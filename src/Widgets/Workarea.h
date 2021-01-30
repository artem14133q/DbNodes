#ifndef WORKAREA_H
#define WORKAREA_H

#include "QWidget"
#include "QWidgetList"
#include "QPointer"
#include "QPushButton"

#include "Node.h"
#include "DeleteArrowButton.h"
#include "config.h"

namespace DbNodes::Widgets {

    class WorkArea : public QWidget
    {
        Q_OBJECT

        public:
            explicit WorkArea(QWidget *parent, const QString &projectName);

            static const int GET_FK_NODE_ROWS = 2;
            static const int GET_PK_NODE_ROWS = 1;

            void makeRelation(const QString &relationId, QPointer<NodeRow> &pkNodeRaw, QPointer<NodeRow> &fkNodeRaw);
            void deleteRelation(const QString &relationName);
            void setNodeRow(QPointer<NodeRow> &);
            void createNodeFromFile(const QString &id, const QString &name, const QPoint &pos);
            void scrollToNode(const QString &nodeId);
            NODE_POINTER findNode(const QString &nodeId);

            QString getProjectName();
            QVector<NODE_POINTER> getAllNodes();
            QVector<QPair<QString, QStringList>> getAllRelations();
            NODE_RAW_POINTER findNodeRow(int type, const QString &nodeRowId);

        private:
            QVector<QPair<QString, QStringList>> relations;
            QVector<QPair<QString, QPointer<DeleteArrowButton>>> closeBtnList;
            QVector<QPointer<NodeRow>> pkList, fkList;
            QVector<QPointer<Node>> nodeList;
            QString projectName;

            void contextMenuEvent(QContextMenuEvent *event) override;
            void paintEvent(QPaintEvent *event) override;

            void cleanNodeList();
            void createNode(const QPoint &pos);
            static void cleanNodeRowsList(QVector<QPointer<NodeRow>> &list);

            bool isAntialiasing;

            #if APP_DEBUG

            void debugRelation();

            #endif
    };

}

#endif // WORKAREA_H
