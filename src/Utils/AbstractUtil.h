//
// Created by artem on 15.04.2021.
//

#ifndef DBNODES_ABSTRACTUTIL_H
#define DBNODES_ABSTRACTUTIL_H

#include "QObject"
#include "QMenu"

namespace DbNodes::Utils {

    class AbstractUtil: public QObject
    {
        Q_OBJECT

        public:
            explicit AbstractUtil(QObject *object = nullptr);

            virtual void initDefaultActionsForUtil(QMenu *menu);
    };

}


#endif //DBNODES_ABSTRACTUTIL_H
