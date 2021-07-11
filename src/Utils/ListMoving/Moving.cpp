//
// Created by artem on 15.04.2021.
//

#include "Moving.h"
#include "../../helper.h"

namespace DbNodes::Utils::ListMoving
{
    /**
     * Moving Constructor
     * @param moveHandle (widget for drag and drop interactions)
     * @param object
     */
    Moving::Moving(
        QWidget *moveHandle,
        QWidget *parentWidget
    ): AbstractUtil(parentWidget), moveHandle(moveHandle), parentWidget(parentWidget) {}

    /**
     * Prepare target object for drag and drop
     */
    void Moving::enable()
    {
        if (moveHandle->geometry().contains(parentWidget->mapFromGlobal(QCursor::pos()))) {

            parentWidget->setStyleSheet(
                    // TODO: create style file
                parentWidget->styleSheet() + "QWidget{border: 1px solid #f36026;}"
            );

            isMovable = true;
            parentWidget->raise();
        }
    }

    /**
     * Move picked out widget
     * @return status of operation completing
     */
    bool Moving::move()
    {
        if (isMovable) {
            Helper::updateParentRecursive(parentWidget, "WorkArea");
            return true;
        }

        return false;
    }

    /**
     * Stop moving and start events
     * @param callback (action after stopping moving)
     */
    void Moving::replace(const std::function<void ()> &callback)
    {
        if (isMovable) {
            callback();

            isMovable = false;

            parentWidget->setStyleSheet(
                    // TODO: replace to style file
                parentWidget->styleSheet().split("QWidget{border: 1px solid #f36026;}")[0]
            );
        }

        Helper::updateParentRecursive(parentWidget, "WorkArea");
    }
}
