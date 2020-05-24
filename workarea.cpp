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

    // Define actions
    QAction* createTable = menu->addAction("Create Table");
    QAction* makeRelation = menu->addAction("make Relation");

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
    QVectorIterator<QPair<QString, QVector<QPointer<NodeRow>>>> relationIterator(this->relations);
    // Loop for paint arrows
    while (relationIterator.hasNext()) {
        QPair<QString, QVector<QPointer<NodeRow>>> relationPair(relationIterator.next());
        QVector<QPointer<NodeRow>> relationArrowList(relationPair.second);

        // If one or two NodeRow widget delete pair
        if (!relationArrowList.first() || !relationArrowList.last()) {
            this->relations.removeOne(relationPair);
            continue;
        }

        QPointer<DeleteArrowButton> closeBtn;

        QVectorIterator<QPair<QString, QPointer<DeleteArrowButton>>>
                relationCloseBtnsIterator(this->closeBtnList);

        while (relationCloseBtnsIterator.hasNext()) {
            QPair<QString, QPointer<DeleteArrowButton>> relationCloseBtnPair(relationCloseBtnsIterator.next());

            if (relationCloseBtnPair.first == relationPair.first)
                closeBtn = relationCloseBtnPair.second;
        }

        // Get coords and widths from NodeRow
        QPair<QPoint, int> pkCoords(relationArrowList.first()->getGlobalPos());
        QPair<QPoint, int> fkCoords(relationArrowList.last()->getGlobalPos());

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
    // Start window for create relation
    new RelationMaker(this, this->pkList, this->fkList);
}

void WorkArea::makeRelation(QString relationName, QPointer<NodeRow> pkNodeRow, QPointer<NodeRow> fkNodeRow)
{
    QVectorIterator<QPair<QString, QVector<QPointer<NodeRow>>>> relationIterator(this->relations);
    while (relationIterator.hasNext()) {
        QVector<QPointer<NodeRow>> relationPair(relationIterator.next().second);
        //If relation pair already in relation list
        if (relationPair.first() == pkNodeRow && relationPair.last() == fkNodeRow)
            return;
        //If relation rows is children of one node
        else if (relationPair.first() == relationPair.last())
            return;
    }

    // Call from relation maker. Append Pointers of NodeRows to relations list
    QVector<QPointer<NodeRow>> newRelationPair;
    QString newRelationName = relationName;

    newRelationPair.insert(0, pkNodeRow);
    newRelationPair.insert(1, fkNodeRow);

    this->relations.push_back(QPair<QString, QVector<QPointer<NodeRow>>>(newRelationName, newRelationPair));
    this->closeBtnList.push_back(QPair<QString, QPointer<DeleteArrowButton>>(
                                     relationName,
                                     QPointer<DeleteArrowButton>(new DeleteArrowButton(relationName, this))
                                 ));
}

void WorkArea::deleteRalation(QString & relationName)
{
    QVectorIterator<QPair<QString, QVector<QPointer<NodeRow>>>> relationIterator(this->relations);
    while (relationIterator.hasNext()) {
        QPair<QString, QVector<QPointer<NodeRow>>> relationPair(relationIterator.next());

        if (relationPair.first == relationName)
            this->relations.removeOne(relationPair);
    }
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

void WorkArea::createNode(QPoint pos)
{
    Node* node = new Node(this);
    node->move(pos);
}
