#include "QMenu"
#include "QContextMenuEvent"
#include "QPoint"
#include "QVector"
#include "QPainter"
#include "QPen"
#include "QColor"
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
        // Init new Pen with color
        QColor color;
        color.setRgb(47, 167, 227);

        // Init painter
        QPainter painter(this);
        // Set antialiasing for arrows

        if (isAntialiasing) painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(color, 2, Qt::SolidLine, Qt::FlatCap));

        // Init path class for arrow
        QPainterPath bezierPath;

        // Paint relations for NodeRows
        QVectorIterator<QPair<QString, QStringList>> relationIterator(relations);
        // Loop for paint arrows
        while (relationIterator.hasNext()) {
            auto relationPair(relationIterator.next());
            QStringList relationArrowList(relationPair.second);

            // Find button to move or delete
            QPointer<DeleteArrowButton> closeBtn;
            QVectorIterator<QPair<QString, QPointer<DeleteArrowButton>>> relationCloseBtnsIterator(closeBtnList);
            while (relationCloseBtnsIterator.hasNext()) {
                auto relationCloseBtnPair(relationCloseBtnsIterator.next());
                if (relationCloseBtnPair.first == relationPair.first) closeBtn = relationCloseBtnPair.second;
            }

            cleanNodeRowsList(pkList);
            cleanNodeRowsList(fkList);

            QPointer<NodeRow> pkNodeRow(findNodeRow(WorkArea::GET_PK_NODE_ROWS, relationArrowList.first()));
            QPointer<NodeRow> fkNodeRow(findNodeRow(WorkArea::GET_FK_NODE_ROWS, relationArrowList.last()));

            // If one or two NodeRow widget delete pair
            if (!pkNodeRow || !fkNodeRow) {
                relations.removeOne(relationPair);
                // If arrow was deleted, delete close button
                if (closeBtn) closeBtn->~DeleteArrowButton();

                continue;
            }

            // Get cords and widths from NodeRow
            QPair<QPoint, int> pkCords(pkNodeRow->getGlobalPos());
            QPair<QPoint, int> fkCords(fkNodeRow->getGlobalPos());

            QPoint pkPoint(pkCords.first);
            QPoint fkPoint(fkCords.first);

            // If PK position x < FK position x:
            // Start position PK move to right coord of Node
            if (pkPoint.x() < fkPoint.x()) {
                pkPoint.rx() += pkCords.second;
                closeBtn->move(fkPoint.x() - closeBtn->width() - 2, fkPoint.y() - closeBtn->height() / 2);
                fkPoint.rx() -= (closeBtn->width() + 2);
            }
            // If PK position x > FK position x:
            // Start position FK move to right coord of Node
            else {
                fkPoint.rx() += fkCords.second;
                closeBtn->move(fkPoint.x() + 2, fkPoint.y() - closeBtn->height() / 2);
                fkPoint.rx() += closeBtn->width();
            }

            // Calculate positions of middle points
            int cP12_x = pkPoint.x() + (fkPoint.x() - pkPoint.x())/2;

            // Set cords
            bezierPath.moveTo(pkPoint.x(), pkPoint.y());
            bezierPath.cubicTo(cP12_x, pkPoint.y(), cP12_x, fkPoint.y(), fkPoint.x(), fkPoint.y());

            // Paint arrow
            painter.drawPath(bezierPath);
        }
    }

    void WorkArea::makeRelation(const QString &relationId, QPointer<NodeRow> &pkNodeRaw, QPointer<NodeRow> &fkNodeRaw)
    {
        if (pkNodeRaw->getTableId() == fkNodeRaw->getTableId()) return;

        QVectorIterator<QPair<QString, QStringList>> relationIterator(relations);
        while (relationIterator.hasNext()) {
            QStringList relationList(relationIterator.next().second);
            // If relation pair already in relation list
            if (relationList.first() == pkNodeRaw->getRowId() && relationList.last() == fkNodeRaw->getRowId()) return;

            // If relation rows is children of one node
            if (relationList.first() == relationList.last()) return;
        }

        // Call from relation maker. Append Pointers of NodeRows to relations list
        QStringList newRelationArrowList;
        const QString& newRelationName = relationId;

        newRelationArrowList.insert(0, pkNodeRaw->getRowId());
        newRelationArrowList.insert(1, fkNodeRaw->getRowId());

        QPointer<DeleteArrowButton> newDeleteBtn(new DeleteArrowButton(relationId, this));

        fkNodeRaw->disableFkRelationButton(true);

        relations.push_back(QPair<QString, QStringList>(newRelationName, newRelationArrowList));
        closeBtnList.push_back(QPair<QString, QPointer<DeleteArrowButton>>(relationId, newDeleteBtn));
    }

    void WorkArea::deleteRelation(const QString &relationName)
    {
        QVectorIterator<QPair<QString, QStringList>> relationIterator(relations);
        while (relationIterator.hasNext()) {
            QPair<QString, QStringList> relationPair(relationIterator.next());
            if (relationPair.first == relationName) {
                findNodeRow(NodeRow::FK, relationPair.second.last())->disableFkRelationButton(false);
                relations.removeOne(relationPair);
            };
        }
    }

    QPointer<NodeRow> WorkArea::findNodeRow(int type, const QString &nodeRowId)
    {
        QVector<QPointer<NodeRow>> nodeRows;

        if (type == WorkArea::GET_PK_NODE_ROWS)
            nodeRows = pkList;
        else if (type == WorkArea::GET_FK_NODE_ROWS)
            nodeRows = fkList;

        foreach (QPointer<NodeRow> nodeRow, nodeRows) {
            if (nodeRow->getRowId() == nodeRowId) return nodeRow;
        }

        return QPointer<NodeRow>(nullptr);
    }

// Clean NodeRows list for delete pointer if pointer is empty
    void WorkArea::cleanNodeRowsList(QVector<QPointer<NodeRow>> &list)
    {
        QVectorIterator<QPointer<NodeRow>> listIterator(list);
        while (listIterator.hasNext()) {
            QPointer<NodeRow> nodeRow = listIterator.next();
            if (!nodeRow) list.removeOne(nodeRow);
        }
    }

    void WorkArea::cleanNodeList()
    {
        QVectorIterator<QPointer<Node>> nodeListIterator(nodeList);
        while (nodeListIterator.hasNext()) {
            QPointer<Node> node(nodeListIterator.next());
            if (!node) nodeList.removeOne(node);
        }
    }

    void WorkArea::setNodeRow(QPointer<NodeRow> &nodeRow)
    {
        if (nodeRow->getRowType() == NodeRow::PK) pkList.push_back(nodeRow);
        else if (nodeRow->getRowType() == NodeRow::FK) fkList.push_back(nodeRow);
    }

    void WorkArea::createNode(const QPoint &pos)
    {
        Node* node = new Node(this);
        nodeList.push_back(QPointer<Node>(node));
        node->move(pos);
    }

    void WorkArea::createNodeFromFile(const QString &id, const QString &name, const QPoint &pos)
    {
        QPointer<Node> node = new Node(this, id, name);
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

    QVector<QPair<QString, QStringList>> WorkArea::getAllRelations()
    {
        return relations;
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
        QVectorIterator<QPair<QString, QStringList>> relationsIterator(getAllRelations());
        for (int i = 0; relationsIterator.hasNext(); ++i) {
            auto relationList = relationsIterator.next().second;
            QPointer<NodeRow> pkNodeRow(findNodeRow(WorkArea::GET_PK_NODE_ROWS, relationList.first()));
            QPointer<NodeRow> fkNodeRow(findNodeRow(WorkArea::GET_FK_NODE_ROWS, relationList.last()));

            qDebug() << i
                << (i < 10 ? " " : "")
                << pkNodeRow->getTableName() + "::" + pkNodeRow->getRowName()
                << " <-- "
                << fkNodeRow->getTableName() + "::" + fkNodeRow->getRowName();
        }
        qDebug() << "============== DEBUG ALL RELATIONS =================";
    }

#endif
}
