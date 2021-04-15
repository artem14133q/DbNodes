//
// Created by artem on 15.04.2021.
//

#include "Moving.h"
#include "../../helper.h"

namespace DbNodes::Utils::ListMoving
{
    Moving::Moving(QWidget *moveHandle, QObject *object): AbstractUtil(object), moveHandle(moveHandle) {}

    void Moving::enable()
    {
        if (moveHandle->geometry().contains(parentWidget()->mapFromGlobal(QCursor::pos()))) {

            parentWidget()->setStyleSheet(
                parentWidget()->styleSheet() + "QWidget{border: 1px solid #f36026;}"
            );

            isMovable = true;
            parentWidget()->raise();
        }
    }

    QWidget *Moving::parentWidget()
    {
        return dynamic_cast<QWidget *>(parent());
    }

    bool Moving::move()
    {
        if (isMovable) {
            Helper::updateParentRecursive(parentWidget(), "WorkArea");
            return true;
        }

        return false;
    }

    void Moving::replace(const std::function<void ()> &callback)
    {
        if (isMovable) {
            callback();

            isMovable = false;

            parentWidget()->setStyleSheet(
                parentWidget()->styleSheet().split("QWidget{border: 1px solid #f36026;}")[0]
            );
        }

        Helper::updateParentRecursive(parentWidget(), "WorkArea");
    }
}
