//
// Created by artem on 18.04.2021.
//

#include "QPainter"
#include "QStyleOption"

#include "MinimapWorkingArea.h"
#include "TableNode.h"
#include "QDebug"

#include "../helper.h"

namespace DbNodes::Widgets::Minimap
{
    MinimapWorkingArea::MinimapWorkingArea(
        const QList<Abstract::NodePtr> &nodes,
        QLabel *nodeTitle,
        const float &scaleCoefficient,
        QWidget *parent
    ): QWidget(parent), nodes(nodes), scaleCoefficient(scaleCoefficient), nodeTitle(nodeTitle)
    {
        setObjectName("MinimapWorkingArea");
        setStyleSheet(Helper::getStyleFromFile("minimapWorkingArea"));

        movableRect = new MovableRect(scaleCoefficient, this);
        connect(movableRect, &MovableRect::moveToPositionSignal, this, &MinimapWorkingArea::moveToPosition);

        tablesColor = QColor(0, 109, 0);
        focusedTablesColor = QColor(0, 183, 235);
        selectedTablesColor = QColor(243, 97, 39);

        setMouseTracking(true);
    }

    void MinimapWorkingArea::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        QStyleOption opt;
        opt.init(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

        QPen pen(QPen(tablesColor, 1, Qt::SolidLine, Qt::FlatCap));

        bool showNodeName = false;

        foreach (const Abstract::NodePtr &node, nodes) {
            if (node->objectName() == "TableNode") {
                auto nodeLocalRect = QRect(
                    node->x() / scaleCoefficient,
                    node->y() / scaleCoefficient,
                    node->width() / scaleCoefficient,
                    node->height() / scaleCoefficient
                );

                bool dampFocus = node->hasFocus();

                if (dampFocus) {
                    pen.setColor(focusedTablesColor);
                } else if (node->property("selected").toBool()) {
                    pen.setColor(selectedTablesColor);
                } else {
                    pen.setColor(tablesColor);
                }

                painter.setPen(pen);
                painter.drawRect(nodeLocalRect);

                auto cursorLocalPos = mapFromGlobal(QCursor::pos());

                if (!underMouse()) {
                    continue;
                }

                bool workingWithFocusAvailable = movableRect->workingWithFocusAvailable();

                if (nodeLocalRect.contains(cursorLocalPos)) {
                    showNodeName = true;

                    Abstract::AbstractNode *tableNode(node);

                    auto tableName = dynamic_cast<Nodes::TableNode *>(tableNode)->getTableName();

                    nodeTitle->setText(tableName);
                    nodeTitle->adjustSize();

                    if (workingWithFocusAvailable) node->setFocus();
                } else {
                    if (showNodeName) continue;

                    nodeTitle->setText("");

                    if (workingWithFocusAvailable && (!dampFocus || underMouse())) {
                        node->clearFocus();
                    }
                }
            }
        }
    }

    void MinimapWorkingArea::moveToPosition(const QPoint &pos)
    {
        emit moveToPositionSignal(pos * scaleCoefficient);
    }

    void MinimapWorkingArea::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            movableRect->restrictedMove(
                event->pos().x() - movableRect->width() / 2,
                event->pos().y() - movableRect->height() / 2
            );
        }

        moveToPosition(movableRect->pos());
    }

    void MinimapWorkingArea::mouseMoveEvent(QMouseEvent *event)
    {
        update();

        QWidget::mouseMoveEvent(event);
    }
}