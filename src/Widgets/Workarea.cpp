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
#include "Node.h"
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
        QAction* createTable = menu->addAction("Create Table");

        // Define slots
        connect(createTable, &QAction::triggered, this, [this, event] {
            createNode(event->pos());
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
        NODE_RAW_POINTER &pkNodeRaw,
        NODE_RAW_POINTER &fkNodeRaw
    ) {
        if (pkNodeRaw->getTableId() == fkNodeRaw->getTableId()) return nullptr;

        foreach (const RELATION_POINTER &relation, relations) {
            if (relation->getPkNodeRaw()->getRowId() == pkNodeRaw->getRowId()
                && relation->getFkNodeRaw()->getRowId() == fkNodeRaw->getRowId())
                return nullptr;

            if (relation->getPkNodeRaw() == relation->getFkNodeRaw()) return nullptr;
        }

        fkNodeRaw->disableFkRelationButton(true);

        RELATION_POINTER relation(
            new Relations::Relation(this, relationId, relationType, pkNodeRaw, fkNodeRaw)
        );

        connect(relation, &Relations::Relation::goToRelatedTable, this, &WorkArea::scrollToNode);

        relations.append(relation);

        dynamic_cast<Node *>(fkNodeRaw->parentWidget())->addRelation(relation);

        return relation;
    }

    NODE_RAW_POINTER WorkArea::findNodeRow(int type, const QString &nodeRowId)
    {
        QVector<NODE_RAW_POINTER> nodeRows;

        if (type == WorkArea::GET_PK_NODE_ROWS)
            nodeRows = pkList;
        else if (type == WorkArea::GET_FK_NODE_ROWS)
            nodeRows = fkList;

        foreach (const NODE_RAW_POINTER &nodeRow, nodeRows) {
            if (nodeRow->getRowId() == nodeRowId) return nodeRow;
        }

        return NODE_RAW_POINTER(nullptr);
    }

    // Clean NodeRows list for delete pointer if pointer is empty
    void WorkArea::cleanNodeRowsList(QVector<NODE_RAW_POINTER> &list)
    {
        QVectorIterator<NODE_RAW_POINTER> listIterator(list);
        while (listIterator.hasNext()) {
            auto nodeRow = listIterator.next();
            if (!nodeRow) list.removeOne(nodeRow);
        }
    }

    void WorkArea::setNodeRaw(NODE_RAW_POINTER &nodeRaw)
    {
        if (nodeRaw->getRowType() == NodeRow::PK)
            pkList.push_back(nodeRaw);
        else if (nodeRaw->getRowType() == NodeRow::FK)
            fkList.push_back(nodeRaw);
    }

    NODE_POINTER WorkArea::createNode(const QPoint &pos, const QString &id, const QString &name)
    {
        NODE_POINTER node;

        if (id == nullptr || name == nullptr) {
            node = new Node(this);
        } else {
            node = new Node(this, id, name);
        }

        nodeList.push_back(node);
        node->move(pos);

        selectionRepository->initDefaultsConnections(node);

        return node;
    }

    QList<NODE_POINTER> WorkArea::getAllNodes()
    {
        Helper::removeDeletedItems<Node>(nodeList);

        cleanNodeRowsList(pkList);
        cleanNodeRowsList(fkList);

        return nodeList;
    }

    QString WorkArea::getProjectName()
    {
        return projectName;
    }

    void WorkArea::scrollToNode(const QString &nodeId)
    {
        NODE_POINTER node = findNode(nodeId);

        auto *mainWindow = Helper::findParentWidgetRecursive(this, "MainWindow");

        int y = (node->height() < mainWindow->height())
                ? node->y() - mainWindow->height() / 2 + node->height() / 2
                : node->y();

        int x = node->x() - mainWindow->width() / 2 + node->width() / 2;

        scrollToPosition(QPoint(x, y));

        QTimer::singleShot(0, node, SLOT (setFocus()));
        node->raise();
    }

    NODE_POINTER WorkArea::findNode(const QString &nodeId)
    {
        foreach (NODE_POINTER node, getAllNodes()) {
            if (node->getTableId() == nodeId) {
                return node;
            }
        }

        return nullptr;
    }

#if APP_DEBUG

    void WorkArea::debugRelation()
    {
        qDebug() << "============== DEBUG ALL RELATIONS =================";
        for (int i = 0; i < relations.size(); ++i) {
            auto pkNodeRow(relations[i]->getPkNodeRaw());
            auto fkNodeRow(relations[i]->getFkNodeRaw());

            qDebug() << i
                << (i < 10 ? " " : "")
                << pkNodeRow->getTableName() + "::" + pkNodeRow->getRowName()
                << " <-- "
                << fkNodeRow->getTableName() + "::" + fkNodeRow->getRowName();
        }
        qDebug() << "============== DEBUG ALL RELATIONS =================";
    }

#endif

    WorkArea::~WorkArea()
    {
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
        auto *scrollWidget = static_cast<QScrollArea *>(parentWidget()->parentWidget());

        scrollWidget->verticalScrollBar()->setValue(pos.y());
        scrollWidget->horizontalScrollBar()->setValue(pos.x());
    }
}
