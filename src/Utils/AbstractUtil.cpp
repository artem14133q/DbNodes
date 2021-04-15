//
// Created by artem on 15.04.2021.
//

#include "AbstractUtil.h"

namespace DbNodes::Utils
{
    AbstractUtil::AbstractUtil(QObject *object): QObject(object) {}

    void AbstractUtil::initDefaultActionsForUtil(QMenu *menu) {}
}