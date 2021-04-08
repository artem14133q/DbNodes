#ifndef WORKAREA_H
#define WORKAREA_H

#include "QWidget"
#include "QWidgetList"
#include "QPointer"
#include "QPushButton"
#include "QMouseEvent"

#include "TableNode.h"
#include "config.h"
#include "Relation.h"
#include "MultipleSelection/Repository.h"

namespace DbNodes::Widgets {

    class WorkArea : public QWidget
    {
        Q_OBJECT

        public:
            explicit WorkArea(QWidget *parent);

            static const int GET_FK_COLUMNS = 2;
            static const int GET_PK_COLUMNS = 1;

            RELATION_POINTER makeRelation(
                const QString &relationId,
                const int &relationType,
                COLUMN_POINTER &pkColumn,
                COLUMN_POINTER &fkColumn
            );

            void setColumn(COLUMN_POINTER &column);
            void scrollToTable(const QString &columnId);
            void scrollToPosition(const QPoint &pos);
            TABLE_POINTER findTable(const QString &columnId);

            QString getProjectName();
            void setProjectName(const QString &name);
            QList<TABLE_POINTER> getAllTables();
            COLUMN_POINTER findColumn(int type, const QString &columnId);

            TABLE_POINTER createTable(
                const QPoint &pos,
                const QString &id = nullptr,
                const QString &name = nullptr
            );

            const QList<RELATION_POINTER> &getAllRelations();

            ~WorkArea() override;

        private:
            Utils::MultipleSelection::Repository *selectionRepository;

            QList<RELATION_POINTER> relations;
            QVector<COLUMN_POINTER> pkList, fkList;
            QList<TABLE_POINTER> tableList;
            QString projectName;

            void contextMenuEvent(QContextMenuEvent *event) override;
            void paintEvent(QPaintEvent *event) override;

            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            static void cleanColumnList(QVector<COLUMN_POINTER> &list);

            bool isAntialiasing;

            #if APP_DEBUG

            void debugRelation();

            #endif
    };

}

#endif // WORKAREA_H
