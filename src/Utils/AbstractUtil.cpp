//
// Created by artem on 15.04.2021.
//

#include "AbstractUtil.h"

namespace DbNodes::Utils
{
    /**
     * AbstractUtil Constructor
     * @param object
     */
    AbstractUtil::AbstractUtil(QObject *object): QObject(object) {}

    /**
     * Init default menu item for utils
     * @param menu (target menu instance)
     */
    void AbstractUtil::initDefaultActionsForUtil(QMenu *menu) {}
}