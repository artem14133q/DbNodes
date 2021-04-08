#include "QMenu"
#include "QContextMenuEvent"
#include "QPoint"
#include "QVector"
#include "QPainter"
#include "QPen"
#include "QPainterPath"
#include "QDebug"
#include "QScrollArea"
#include "QScrollBar"
#include "QTimer"
#include "QStyle"
#include "QApplication"

#include "Workarea.h"
#include "TableNode.h"
#include "RelationTypesDictionary.h"

#include "../helper.h"

namespace DbNodes::Widgets {

    WorkArea::WorkArea(QWidget *parent): QWidget(parent)
    {
        setObjectName("WorkArea");
        // Set fixed size for work area
        setFixedSize(20000, 10000);

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
        foreach (const RELATION_POINTER &relation, relations) {
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

    RELATION_POINTER WorkArea::makeRelation(
            const QString &relationId,
            const int &relationType,
            COLUMN_POINTER &pkColumn,
            COLUMN_POINTER &fkColumn
    ) {
        if (pkColumn->getTableId() == fkColumn->getTableId()) return nullptr;

        foreach (const RELATION_POINTER &relation, relations) {
            if (relation->getPkColumn()->getColumnId() == pkColumn->getColumnId()
                && relation->getFkColumn()->getColumnId() == fkColumn->getColumnId())
                return nullptr;

            if (relation->getPkColumn() == relation->getFkColumn()) return nullptr;
        }

        fkColumn->disableFkRelationButton(true);

        RELATION_POINTER relation(
            new Relations::Relation(this, relationId, relationType, pkColumn, fkColumn)
        );

        connect(relation, &Relations::Relation::goToRelatedTable, this, &WorkArea::scrollToTable);

        relations.append(relation);

        dynamic_cast<Nodes::TableNode *>(fkColumn->parentWidget())->addRelation(relation);

        return relation;
    }

    COLUMN_POINTER WorkArea::findColumn(int type, const QString &columnId)
    {
        QVector<COLUMN_POINTER> columns;

        if (type == WorkArea::GET_PK_COLUMNS)
            columns = pkList;
        else if (type == WorkArea::GET_FK_COLUMNS)
            columns = fkList;

        foreach (const COLUMN_POINTER &column, columns) {
            if (column->getColumnId() == columnId) return column;
        }

        return COLUMN_POINTER(nullptr);
    }

    // Clean Columns list for delete pointer if pointer is empty
    void WorkArea::cleanColumnList(QVector<COLUMN_POINTER> &list)
    {
        QVectorIterator<COLUMN_POINTER> listIterator(list);
        while (listIterator.hasNext()) {
            auto column = listIterator.next();
            if (!column) list.removeOne(column);
        }
    }

    void WorkArea::setColumn(COLUMN_POINTER &column)
    {
        if (column->getColumnType() == Nodes::Table::Column::PK)
            pkList.push_back(column);
        else if (column->getColumnType() == Nodes::Table::Column::FK)
            fkList.push_back(column);
    }

    TABLE_POINTER WorkArea::createTable(const QPoint &pos, const QString &id, const QString &name)
    {
        TABLE_POINTER table;

        if (id == nullptr || name == nullptr) {
            table = new Nodes::TableNode(this);
        } else {
            table = new Nodes::TableNode(this, id, name);
        }

        tableList.push_back(table);
        table->move(pos);

        selectionRepository->initDefaultsConnections(table);

        return table;
    }

    QList<TABLE_POINTER> WorkArea::getAllTables()
    {
        Helper::removeDeletedItems<Nodes::TableNode>(tableList);

        cleanColumnList(pkList);
        cleanColumnList(fkList);

        return tableList;
    }

    QString WorkArea::getProjectName()
    {
        return projectName;
    }

    void WorkArea::scrollToTable(const QString &tableId)
    {
        TABLE_POINTER table = findTable(tableId);

        auto *mainWindow = Helper::findParentWidgetRecursive(this, "MainWindow");

        int y = (table->height() < mainWindow->height())
                ? table->y() - mainWindow->height() / 2 + table->height() / 2
                : table->y();

        int x = table->x() - mainWindow->width() / 2 + table->width() / 2;

        scrollToPosition(QPoint(x, y));

        QTimer::singleShot(0, table, SLOT (setFocus()));
        table->raise();
    }

    TABLE_POINTER WorkArea::findTable(const QString &tableId)
    {
        foreach (TABLE_POINTER table, getAllTables()) {
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

    const QList<RELATION_POINTER> &WorkArea::getAllRelations()
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
        selectionRepository->move(event->pos(), getAllTables());
    }

    void WorkArea::mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            selectionRepository->stop();
        }
    }

    void WorkArea::scrollToPosition(const QPoint &pos)
    {
        auto *scrollWidget = static_cast<QScrollArea *>(parentWidget()->parentWidget());

        scrollWidget->verticalScrollBar()->setValue(pos.y());
        scrollWidget->horizontalScrollBar()->setValue(pos.x());
    }
}
