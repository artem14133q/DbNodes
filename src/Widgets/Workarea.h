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

#include "RelationTypesDictionary.h"
#include "Minimap/MinimapWidget.h"

#include "MinimapPositionsDictionary.h"

namespace DbNodes::Widgets {

    class WorkArea : public QWidget
    {
        Q_OBJECT

        public:
            explicit WorkArea(QWidget *parent);

            static const int GET_FK_COLUMNS = 2;
            static const int GET_PK_COLUMNS = 1;

            Relations::RelationPtr makeRelation(
                const QString &relationId,
                const Dictionaries::RelationTypesDictionary::Type &relationType,
                Nodes::Table::ColumnPrt &pkColumn,
                Nodes::Table::ColumnPrt &fkColumn
            );

            void setColumn(Nodes::Table::ColumnPrt &column);
            void scrollToTable(const QString &columnId);
            void scrollToPosition(const QPoint &pos);
            Nodes::TablePtr findTable(const QString &columnId);

            QString getProjectName();
            void setProjectName(const QString &name);
            QList<Nodes::TablePtr> getAllTables();
            QList<Abstract::NodePtr> getAllNodes();
            Nodes::Table::ColumnPrt findColumn(int type, const QString &columnId);

            Nodes::TablePtr createTable(
                const QPoint &pos,
                const QString &id = nullptr,
                const QString &name = nullptr
            );

            const QList<Relations::RelationPtr> &getAllRelations();

            void createMinimap();

            ~WorkArea() override;

        private:
            Utils::MultipleSelection::Repository *selectionRepository;

            Minimap::MinimapWidget *minimap{};

            QList<Relations::RelationPtr> relations;
            Nodes::Table::ColumnPrtVector pkList, fkList;
            QList<Abstract::NodePtr> nodeList;
            QString projectName;

            void contextMenuEvent(QContextMenuEvent *event) override;
            void paintEvent(QPaintEvent *event) override;

            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            static void cleanColumnList(Nodes::Table::ColumnPrtVector &list);

            bool isAntialiasing;
    };

}

#endif // WORKAREA_H
