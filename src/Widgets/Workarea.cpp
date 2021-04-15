#include "QMenu"
#include "QContextMenuEvent"
#include "QPoint"
#include "QVector"
#include "QPainter"
#include "QPainterPath"
#include "QDebug"
#include "QScrollArea"
#include "QScrollBar"
#include "QTimer"
#include "QStyle"
#include "QApplication"

#include "Workarea.h"
#include "TableNode.h"
#include "RelationViews/Path/PathPoint.h"

#include "../helper.h"

namespace DbNodes::Widgets {

    WorkArea::WorkArea(QWidget *parent): QWidget(parent)
    {
        setObjectName("WorkArea");
        // Set fixed size for work area

        selectionRepository = new Utils::MultipleSelection::Repository(this);

        isAntialiasing = Helper::getSettingValue("rendering.antialiasing").toBool();

        Helper::subscribeSettingUpdate("rendering.antialiasing", [this] (const QVariant &value) {
            isAntialiasing = value.toBool();
            update();
        });
    }

    void WorkArea::contextMenuEvent(QContextMenuEvent* event)
    {
        // Define context menu
        auto *menu = new QMenu();
        menu->setStyleSheet(Helper::getStyleFromFile("workAreaMenu"));

        // Define actions
        QAction* createTableAction = menu->addAction("Create Table");

        // Define slots
        connect(createTableAction, &QAction::triggered, this, [this, event] {
            createTable(event->pos());
        });

        selectionRepository->initDefaultActionsForUtil(menu);

        #if APP_DEBUG

        QAction* debugRelations = menu->addAction("[DEBUG] - SHOW ALL RELATIONS");

        connect(debugRelations, &QAction::triggered, this, &WorkArea::debugRelation);

        #endif

        // Create Menu
        menu->exec(mapToGlobal(event->pos()));
    }

    void WorkArea::paintEvent(QPaintEvent *)
    {
        // Init painter
        QPainter painter(this);
        // Set antialiasing for arrows

        if (isAntialiasing) painter.setRenderHint(QPainter::Antialiasing, true);

        // Init path class for arrow
        QPainterPath bezierPath;

        // Loop for paint arrows
        foreach (const Relations::RelationPtr &relation, relations) {
            if (!relation) {
                relations.removeAt(relations.indexOf(relation));
                continue;
            }

            if (!relation->checkIsRelationValid()) {
                relations.removeAt(relations.indexOf(relation));
                delete relation;

                continue;
            }

            relation->updateRelation(painter, bezierPath);
        }

        selectionRepository->drawSelectionRect(painter);
    }

    Relations::RelationPtr WorkArea::makeRelation(
            const QString &relationId,
            const Dictionaries::RelationTypesDictionary::Type &relationType,
            Nodes::Table::ColumnPrt &pkColumn,
            Nodes::Table::ColumnPrt &fkColumn
    ) {
        if (pkColumn->getTableId() == fkColumn->getTableId()) return nullptr;

        foreach (const Relations::RelationPtr &relation, relations) {
            if (relation->getPkColumn()->getColumnId() == pkColumn->getColumnId()
                && relation->getFkColumn()->getColumnId() == fkColumn->getColumnId())
                return nullptr;

            if (relation->getPkColumn() == relation->getFkColumn()) return nullptr;
        }

        fkColumn->disableFkRelationButton(true);

        Relations::RelationPtr relation(
            new Relations::Relation(this, relationId, relationType, pkColumn, fkColumn)
        );

        connect(relation, &Relations::Relation::goToRelatedTable, this, &WorkArea::scrollToTable);

        connect(
            relation,
            &Relations::Relation::createNodeInWorkAreaSignal,
            this,
            [this] (Abstract::AbstractNode *node) {
                nodeList.push_back(QPointer<Abstract::AbstractNode>(node));
                selectionRepository->initDefaultsConnections(node);
            }
        );

        relations.append(relation);

        dynamic_cast<Nodes::TableNode *>(fkColumn->parentWidget())->addRelation(relation);

        return relation;
    }

    Nodes::Table::ColumnPrt WorkArea::findColumn(int type, const QString &columnId)
    {
        Nodes::Table::ColumnPrtVector columns;

        if (type == WorkArea::GET_PK_COLUMNS)
            columns = pkList;
        else if (type == WorkArea::GET_FK_COLUMNS)
            columns = fkList;

        foreach (const Nodes::Table::ColumnPrt &column, columns) {
            if (column->getColumnId() == columnId) return column;
        }

        return Nodes::Table::ColumnPrt(nullptr);
    }

    // Clean Columns list for delete pointer if pointer is empty
    void WorkArea::cleanColumnList(Nodes::Table::ColumnPrtVector &list)
    {
        QVectorIterator<Nodes::Table::ColumnPrt> listIterator(list);
        while (listIterator.hasNext()) {
            auto column = listIterator.next();
            if (!column) list.removeOne(column);
        }
    }

    void WorkArea::setColumn(Nodes::Table::ColumnPrt &column)
    {
        if (column->getColumnType() == Nodes::Table::Column::Type::PrimaryKey)
            pkList.push_back(column);
        else if (column->getColumnType() == Nodes::Table::Column::Type::ForeignKey)
            fkList.push_back(column);
    }

    Nodes::TablePtr WorkArea::createTable(const QPoint &pos, const QString &id, const QString &name)
    {
        Nodes::TablePtr table;

        if (id == nullptr || name == nullptr) {
            table = new Nodes::TableNode(this);
        } else {
            table = new Nodes::TableNode(this, id, name);
        }

        nodeList.push_back(table->toNode());
        table->move(pos);

        selectionRepository->initDefaultsConnections(table->toNode());

        return table;
    }

    QList<Nodes::TablePtr> WorkArea::getAllTables()
    {
        QList<Nodes::TablePtr> tables;

        foreach (const Abstract::NodePtr &node, getAllNodes()) {
            if (node->objectName() == "TableNode") {
                tables.push_back(qobject_cast<Nodes::TableNode *>(node));
            }
        }

        cleanColumnList(pkList);
        cleanColumnList(fkList);

        return tables;
    }

    QList<Abstract::NodePtr> WorkArea::getAllNodes()
    {
        Helper::removeDeletedItems<Abstract::AbstractNode>(nodeList);

        return nodeList;
    }

    QString WorkArea::getProjectName()
    {
        return projectName;
    }

    void WorkArea::scrollToTable(const QString &tableId)
    {
        Nodes::TablePtr table = findTable(tableId);

        auto *mainWindow = Helper::findParentWidgetRecursive(this, "MainWindow");

        int y = (table->height() < mainWindow->height())
                ? table->y() - mainWindow->height() / 2 + table->height() / 2
                : table->y();

        int x = table->x() - mainWindow->width() / 2 + table->width() / 2;

        scrollToPosition(QPoint(x, y));

        QTimer::singleShot(0, table, SLOT (setFocus()));
        table->raise();
    }

    Nodes::TablePtr WorkArea::findTable(const QString &tableId)
    {
        foreach (Nodes::TablePtr table, getAllTables()) {
            if (table->getTableId() == tableId) {
                return table;
            }
        }

        return nullptr;
    }

#if APP_DEBUG

    void WorkArea::debugRelation()
    {
        qDebug() << "============== DEBUG ALL RELATIONS =================";
        for (int i = 0; i < relations.size(); ++i) {
            auto pkColumn(relations[i]->getPkColumn());
            auto fkColumn(relations[i]->getFkColumn());

            qDebug() << i
                << (i < 10 ? " " : "")
                << pkColumn->getTableName() + "::" + pkColumn->getColumnName()
                << " <-- "
                << fkColumn->getTableName() + "::" + fkColumn->getColumnName();
        }
        qDebug() << "============== DEBUG ALL RELATIONS =================";
    }

#endif

    WorkArea::~WorkArea()
    {
        delete selectionRepository;

        Helper::unBindSetting("rendering.antialiasing");

        QWidget::deleteLater();
    }

    const QList<Relations::RelationPtr> &WorkArea::getAllRelations()
    {
        return relations;
    }

    void WorkArea::setProjectName(const QString &name)
    {
        projectName = name;
    }

    void WorkArea::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            selectionRepository->start(event->pos());
        }
    }

    void WorkArea::mouseMoveEvent(QMouseEvent *event)
    {
        selectionRepository->move(event->pos(), getAllNodes());
    }

    void WorkArea::mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            selectionRepository->stop();
        }
    }

    void WorkArea::scrollToPosition(const QPoint &pos)
    {
        auto *scrollWidget = dynamic_cast<QScrollArea *>(parentWidget()->parentWidget());

        scrollWidget->verticalScrollBar()->setValue(pos.y());
        scrollWidget->horizontalScrollBar()->setValue(pos.x());
    }
}
