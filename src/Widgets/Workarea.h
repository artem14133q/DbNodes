#ifndef WORKAREA_H
#define WORKAREA_H

#include "QWidget"
#include "QWidgetList"
#include "QPointer"
#include "QPushButton"
#include "QMouseEvent"

#include "Node.h"
#include "config.h"
#include "Relation.h"
#include "MultipleSelection/Repository.h"

namespace DbNodes::Widgets {

    class WorkArea : public QWidget
    {
        Q_OBJECT

        public:
            explicit WorkArea(QWidget *parent);

            static const int GET_FK_NODE_ROWS = 2;
            static const int GET_PK_NODE_ROWS = 1;

            RELATION_POINTER makeRelation(
                const QString &relationId,
                const int &relationType,
                NODE_RAW_POINTER &pkNodeRaw,
                NODE_RAW_POINTER &fkNodeRaw
            );

            void setNodeRaw(NODE_RAW_POINTER &nodeRaw);
            void scrollToNode(const QString &nodeId);
            void scrollToPosition(const QPoint &pos);
            NODE_POINTER findNode(const QString &nodeId);

            QString getProjectName();
            void setProjectName(const QString &name);
            QList<NODE_POINTER> getAllNodes();
            NODE_RAW_POINTER findNodeRow(int type, const QString &nodeRowId);

            NODE_POINTER createNode(
                    const QPoint &pos,
                    const QString &id = nullptr,
                    const QString &name = nullptr
            );

            const QList<RELATION_POINTER> &getAllRelations();

            ~WorkArea() override;

        private:
            Utils::MultipleSelection::Repository *selectionRepository;

            QList<RELATION_POINTER> relations;
            QVector<NODE_RAW_POINTER> pkList, fkList;
            QList<NODE_POINTER> nodeList;
            QString projectName;

            void contextMenuEvent(QContextMenuEvent *event) override;
            void paintEvent(QPaintEvent *event) override;

            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            static void cleanNodeRowsList(QVector<NODE_RAW_POINTER> &list);

            bool isAntialiasing;

            #if APP_DEBUG

            void debugRelation();

            #endif
    };

}

#endif // WORKAREA_H
