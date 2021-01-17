//
// Created by artem on 19.12.2020.
//

#include "AbstractNode.h"

namespace DbNodes::Abstract {

    AbstractNode::AbstractNode(QWidget *parent) : DrawableWidget(parent) {}

    // Set old pos before moving
    void AbstractNode::mousePressEvent(QMouseEvent* event)
    {
        oldPos = event->globalPos();
    }

    void AbstractNode::mouseMoveEvent(QMouseEvent *event)
    {
        QPoint delta(event->globalPos() - oldPos);
        move(delta.x() + x(), delta.y() + y());

        if (moveRestrictions) {
            if (x() < 0)
                move(0, y());

            if (y() < 0)
                move(x(), 0);

            if ((x() + width()) > parentWidget()->width())
                move(parentWidget()->width() - width(), y());

            if ((y() + height()) > parentWidget()->height())
                move(x(), parentWidget()->height() - height());
        }

        oldPos = event->globalPos();
        parentWidget()->update();
    }

    void AbstractNode::enableMoveRestrictions(const bool &enable)
    {
        moveRestrictions = enable;
    }

}
