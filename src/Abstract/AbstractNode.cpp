//
// Created by artem on 19.12.2020.
//

#include "AbstractNode.h"
#include "QDebug"

namespace DbNodes::Abstract {

    AbstractNode::AbstractNode(QWidget *parent) : DrawableWidget(parent)
    {
        setFocusPolicy(Qt::StrongFocus);

        selectable = new Utils::MultipleSelection::Selectable(this);
    }

    // Set old pos before moving
    void AbstractNode::mousePressEvent(QMouseEvent* event)
    {
        selectable->setClicked(true);

        oldPos = event->globalPos();
    }

    void AbstractNode::mouseMoveEvent(QMouseEvent *event)
    {
        QPoint delta(event->globalPos() - oldPos);

        selectable->move(delta);

        restrictedMove(delta.x() + x(), delta.y() + y());

        oldPos = event->globalPos();
        parentWidget()->update();
    }

    void AbstractNode::enableMoveRestrictions(const bool &enable)
    {
        moveRestrictions = enable;
    }

    Utils::MultipleSelection::Selectable *AbstractNode::getSelectionUtil()
    {
        return selectable;
    }

    void AbstractNode::mouseReleaseEvent(QMouseEvent *event)
    {
        selectable->flush();
    }

    void AbstractNode::restrictedMove(int newX, int newY)
    {
        move(newX, newY);

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
    }

    void AbstractNode::restrictedMove(const QPoint &pos)
    {
        restrictedMove(pos.x(), pos.y());
    }

    NodePtr AbstractNode::toNode()
    {
        return this;
    }

    void AbstractNode::createDefaultActions(QMenu *menu)
    {
        deleteNodeAction = menu->addAction("Delete");

        connect(deleteNodeAction, &QAction::triggered, this, &AbstractNode::deleteLater);
    }
}
