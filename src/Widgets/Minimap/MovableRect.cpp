//
// Created by artem on 16.04.2021.
//

#include "QScreen"
#include "QStyle"
#include "QScrollArea"

#include "MovableRect.h"
#include "../helper.h"

namespace DbNodes::Widgets::Minimap
{

    MovableRect::MovableRect(const float &scaleCoefficient, QWidget *parent)
        : AbstractNode(parent), scaleCoefficient(scaleCoefficient)
    {
        setObjectName("MovableRect");
        enableMoveRestrictions(true);
//        rememberPosWhenAbroad(true);

        setProperty("moving", false);

        setStyleSheet(Helper::getStyleFromFile("movableRect"));

        setMouseTracking(true);
    }

    void MovableRect::mouseMoveEvent(QMouseEvent *event)
    {
        emit moveToPositionSignal(pos());

        AbstractNode::mouseMoveEvent(event);

        parentWidget()->update();
    }

    void MovableRect::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            enableWorkingWithFocus = false;

            setProperty("moving", true);

            style()->polish(this);

            AbstractNode::mousePressEvent(event);
        }
    }

    void MovableRect::mouseReleaseEvent(QMouseEvent *event)
    {
        enableWorkingWithFocus = true;

        setProperty("moving", false);

        style()->polish(this);

        AbstractNode::mouseReleaseEvent(event);
    }

    bool MovableRect::workingWithFocusAvailable()
    {
        return enableWorkingWithFocus;
    }

    void MovableRect::calculateSize()
    {
        auto scrollArea = dynamic_cast<QScrollArea *>(Helper::findParentWidgetRecursive(this, "ScrollArea"));

        setFixedSize(scrollArea->viewport()->size() / scaleCoefficient);
    }
}