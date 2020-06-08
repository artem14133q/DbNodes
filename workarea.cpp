#include "iostream"
#include "workarea.h"
#include "qmenu.h"
#include "QContextMenuEvent"
#include "qpoint.h"
#include "node.h"
#include "iostream"
#include "QVBoxLayout"
#include "qvector.h"
#include "qpainter.h"
#include "qpen.h"
#include "qcolor.h"
#include "qpainterpath.h"
#include "QVectorIterator"
#include "relationmaker.h"
#include "helper.h"

WorkArea::WorkArea(QWidget *parent)
    : QWidget(parent)
{
    // Set fixed size for work area
    this->setFixedSize(4096, 4096);
}


void WorkArea::contextMenuEvent(QContextMenuEvent* event)
{
    // Define context menu
    QMenu* menu = new QMenu();
    menu->setStyleSheet(Helper::getStyleFromFile("workAreaMenu"));

    // Define actions
    QAction* createTable = menu->addAction("Create Table");
    QAction* makeRelation = menu->addAction("Make Relation");

    // Define slots
    connect(createTable, &QAction::triggered, this, [this, event] {
        this->createNode(event->pos());
    });
    connect(makeRelation, &QAction::triggered, this, &WorkArea::startRelationMaker);

    // Create Menu
    menu->exec(this->mapToGlobal(event->pos()));
}

void WorkArea::paintEvent(QPaintEvent *)
{
    // Init new Pen with color
    QColor color;
    color.setRgb(47, 167, 227);

    // Init painter
    QPainter painter(this);
    // Set antialiasing for arrows
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color, 2, Qt::SolidLine, Qt::FlatCap));

    // Init path class for arrow
    QPainterPath bezierPath;

    // Paint relations for NodeRows
    QVectorIterator<QPair<QString, QStringList>> relationIterator(this->relations);
    // Loop for paint arrows
    while (relationIterator.hasNext()) {
        QPair<QString, QStringList> relationPair(relationIterator.next());
        QStringList relationArrowList(relationPair.second);

        // Find button to move or delete
        QPointer<DeleteArrowButton> closeBtn;
        QVectorIterator<QPair<QString, QPointer<DeleteArrowButton>>>
                relationCloseBtnsIterator(this->closeBtnList);
        while (relationCloseBtnsIterator.hasNext()) {
            QPair<QString, QPointer<DeleteArrowButton>> relationCloseBtnPair(relationCloseBtnsIterator.next());
            if (relationCloseBtnPair.first == relationPair.first)
                closeBtn = relationCloseBtnPair.second;
        }

        this->cleanNodeRowsList(this->pkList);
        this->cleanNodeRowsList(this->fkList);

        QPointer<NodeRow> pkNodeRow(this->findNodeRow(this->pkList, relationArrowList.first()));
        QPointer<NodeRow> fkNodeRow(this->findNodeRow(this->fkList, relationArrowList.last()));

        // If one or two NodeRow widget delete pair
        if (!pkNodeRow || !fkNodeRow) {
            this->relations.removeOne(relationPair);
            // If arrow was deleted, delete close butoon
            if (closeBtn)
                closeBtn->~DeleteArrowButton();
            continue;
        }

        // Get coords and widths from NodeRow
        QPair<QPoint, int> pkCoords(pkNodeRow->getGlobalPos());
        QPair<QPoint, int> fkCoords(fkNodeRow->getGlobalPos());

        QPoint pkPoint(pkCoords.first);
        QPoint fkPoint(fkCoords.first);

        // If PK position x < FK position x:
        // Start position PK move to right coord of Node
        if (pkPoint.x() < fkPoint.x()) {
            pkPoint.rx() += pkCoords.second;
            closeBtn->move(fkPoint.x() - closeBtn->width() - 2, fkPoint.y() - closeBtn->height()/2);
            fkPoint.rx() -= (closeBtn->width() + 2);
        }
        // If PK position x > FK position x:
        // Start position FK move to right coord of Node
        else {
            fkPoint.rx() += fkCoords.second;
            closeBtn->move(fkPoint.x() + 2, fkPoint.y() - closeBtn->height()/2);
            fkPoint.rx() += closeBtn->width();
        }

        // Calculate positions of middle points
        int cP12_x = pkPoint.x() + (fkPoint.x() - pkPoint.x())/2;;

        // Set coords
        bezierPath.moveTo(pkPoint.x(), pkPoint.y());
        bezierPath.cubicTo(cP12_x, pkPoint.y(), cP12_x, fkPoint.y(), fkPoint.x(), fkPoint.y());

        // Paint arrow
        painter.drawPath(bezierPath);
    }
}

void WorkArea::startRelationMaker()
{
    this->cleanNodeRowsList(this->pkList);
    this->cleanNodeRowsList(this->fkList);
    this->cleanNodeList();
    // Start window for create relation
    new RelationMaker(this, this->pkList, this->fkList, this->relations);
}

void WorkArea::makeRelation(QString relationName, QPointer<NodeRow> pkNodeRow, QPointer<NodeRow> fkNodeRow)
{
    QVectorIterator<QPair<QString, QStringList>> relationIterator(this->relations);
    while (relationIterator.hasNext()) {
        QStringList relationList(relationIterator.next().second);
        //If relation pair already in relation list
        if (relationList.first() == pkNodeRow->getRowId() && relationList.last() == fkNodeRow->getRowId())
            return;
        //If relation rows is children of one node
        else if (relationList.first() == relationList.last())
            return;
    }

    // Call from relation maker. Append Pointers of NodeRows to relations list
    QStringList newRelationArrowList;
    QString newRelationName = relationName;

    newRelationArrowList.insert(0, pkNodeRow->getRowId());
    newRelationArrowList.insert(1, fkNodeRow->getRowId());

    QPointer<DeleteArrowButton> newDeleteBtn(new DeleteArrowButton(relationName, this));

    this->relations.push_back(QPair<QString, QStringList>(newRelationName, newRelationArrowList));
    this->closeBtnList.push_back(QPair<QString, QPointer<DeleteArrowButton>>(relationName, newDeleteBtn));
}

void WorkArea::deleteRalation(QString & relationName)
{
    QVectorIterator<QPair<QString, QStringList>> relationIterator(this->relations);
    while (relationIterator.hasNext()) {
        QPair<QString, QStringList> relationPair(relationIterator.next());

        if (relationPair.first == relationName)
            this->relations.removeOne(relationPair);
    }
}

QPointer<NodeRow> WorkArea::findNodeRow(QVector<QPointer<NodeRow>> &nodeRowList, QString nodeRowId)
{
    QVectorIterator<QPointer<NodeRow>> nodeRowListIterator(nodeRowList);
    while (nodeRowListIterator.hasNext()) {
        QPointer<NodeRow> nodeRow(nodeRowListIterator.next());
        if (nodeRow->getRowId() == nodeRowId)
            return nodeRow;
    }

    return QPointer<NodeRow>(nullptr);
}

// Clean NodeRows list for delete pointer if pointer is empty
void WorkArea::cleanNodeRowsList(QVector<QPointer<NodeRow>> &list)
{
    QVectorIterator<QPointer<NodeRow>> listIterator(list);
    while (listIterator.hasNext()) {
        QPointer<NodeRow> nodeRow = listIterator.next();

        if (!nodeRow)
            list.removeOne(nodeRow);
    }
}

void WorkArea::cleanNodeList()
{
    QVectorIterator<QPointer<Node>> nodeListIterator(this->nodeList);
    while (nodeListIterator.hasNext()) {
        QPointer<Node> node(nodeListIterator.next());
        if (!node)
            this->nodeList.removeOne(node);
    }
}

void WorkArea::setNodeRow(QPointer<NodeRow> nodeRow)
{
    if (nodeRow->getRowType() == NodeRow::PK)
        this->pkList.push_back(nodeRow);
    else if (nodeRow->getRowType() == NodeRow::FK)
        this->fkList.push_back(nodeRow);
}

void WorkArea::createNode(QPoint pos)
{
    Node* node = new Node(this);
    this->nodeList.push_back(QPointer<Node>(node));
    node->move(pos);
}
