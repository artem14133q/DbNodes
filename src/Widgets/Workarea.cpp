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

#include "Workarea.h"
#include "Node.h"

#include "../helper.h"

namespace DbNodes::Widgets {

    WorkArea::WorkArea(QWidget *parent, const QString &pProjectName): QWidget(parent)
    {
        setObjectName("WorkArea");
        projectName = pProjectName;
        // Set fixed size for work area
        setFixedSize(20000, 10000);

        isAntialiasing = Helper::getSettingValue("rendering.antialiasing").toBool();

        Helper::subscribeSetting("rendering.antialiasing", [this] (const QVariant &value) {
            this->isAntialiasing = value.toBool();
            this->update();
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

            relation->paintPathLine(painter, bezierPath);
        }
    }

    void WorkArea::makeRelation(const QString &relationId, NODE_RAW_POINTER &pkNodeRaw, NODE_RAW_POINTER &fkNodeRaw)
    {
        if (pkNodeRaw->getTableId() == fkNodeRaw->getTableId()) return;

        foreach (const RELATION_POINTER &relation, relations) {
            if (relation->getPkNodeRaw()->getRowId() == pkNodeRaw->getRowId()
                && relation->getFkNodeRaw()->getRowId() == fkNodeRaw->getRowId())
                return;

            if (relation->getPkNodeRaw() == relation->getFkNodeRaw()) return;
        }

        fkNodeRaw->disableFkRelationButton(true);

        RELATION_POINTER relation(new Relations::Relation(this, relationId, pkNodeRaw, fkNodeRaw));

        relations.append(relation);
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

    void WorkArea::cleanNodeList()
    {
        QVectorIterator<NODE_POINTER> nodeListIterator(nodeList);
        while (nodeListIterator.hasNext()) {
            auto node(nodeListIterator.next());
            if (!node) nodeList.removeOne(node);
        }
    }

    void WorkArea::setNodeRaw(NODE_RAW_POINTER &nodeRaw)
    {
        if (nodeRaw->getRowType() == NodeRow::PK)
            pkList.push_back(nodeRaw);
        else if (nodeRaw->getRowType() == NodeRow::FK)
            fkList.push_back(nodeRaw);
    }

    void WorkArea::createNode(const QPoint &pos)
    {
        Node* node = new Node(this);
        nodeList.push_back(NODE_POINTER(node));
        node->move(pos);
    }

    void WorkArea::createNodeFromFile(const QString &id, const QString &name, const QPoint &pos)
    {
        NODE_POINTER node = new Node(this, id, name);
        node->move(pos);
        nodeList.push_back(node);
    }

    QVector<NODE_POINTER> WorkArea::getAllNodes()
    {
        cleanNodeList();

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

        auto *scrollWidget = static_cast<QScrollArea *>(parentWidget()->parentWidget());

        auto *mainWindow = Helper::findParentWidgetRecursive(this, "MainWindow");

        int y = (node->height() < mainWindow->height())
                ? node->y() - mainWindow->height() / 2 + node->height() / 2
                : node->y();

        scrollWidget->verticalScrollBar()->setValue(y);
        scrollWidget->horizontalScrollBar()->setValue(node->x() - mainWindow->width() / 2 + node->width() / 2);

        QTimer::singleShot(0, node, SLOT (setFocus()));
    }

    NODE_POINTER WorkArea::findNode(const QString &nodeId)
    {
        foreach (NODE_POINTER node, getAllNodes().toList()) {
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
        for (int i = 0; relations.size() < i; ++i) {
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
}
