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

        isHandled = true;
    }

    void AbstractNode::mouseMoveEvent(QMouseEvent *event)
    {
        if (isHandled) {
            QPoint cursorPos = event->globalPos();

            QPoint delta(cursorPos - oldPos);

            selectable->move(delta);

            if (abroadHandle) {
                auto localCursorPos = parentWidget()->mapFromGlobal(cursorPos);

                if (!geometry().contains(geometry().x(), localCursorPos.y())) {
                    delta.ry() = 0;
                }

                if (!geometry().contains(localCursorPos.x(), geometry().y())) {
                    delta.rx() = 0;
                }
            }

            restrictedMove(delta.x() + x(), delta.y() + y());

            oldPos = cursorPos;
            parentWidget()->update();
        }
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

        isHandled = false;
    }

    void AbstractNode::restrictedMove(int newX, int newY)
    {
        if (moveRestrictions) {
            if (newX < 0)
                newX = 0;

            if (newY < 0)
                newY = 0;

            if ((newX + width()) > parentWidget()->width())
                newX = parentWidget()->width() - width();

            if ((newY + height()) > parentWidget()->height())
                newY =  parentWidget()->height() - height();
        }

        move(newX, newY);
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

        connect(deleteNodeAction, &QAction::triggered, this, [this] {
            emit deleteNodeSignal();
            deleteLater();
        });
    }

    void AbstractNode::rememberPosWhenAbroad(const bool &enable)
    {
        abroadHandle = enable;
    }

    void AbstractNode::emitDelete()
    {
        emit deleteNodeSignal();
    }
}
