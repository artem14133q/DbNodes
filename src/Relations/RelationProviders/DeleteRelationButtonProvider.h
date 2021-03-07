//
// Created by artem on 03.03.2021.
//

#ifndef DBNODES_DELETERELATIONBUTTONPROVIDER_H
#define DBNODES_DELETERELATIONBUTTONPROVIDER_H

#include "RelationProvider.h"

namespace DbNodes::Relations {

    class DeleteRelationButtonProvider: public RelationProvider
    {
        public:
            Abstract::AbstractRelationView *create() override;
    };

}


#endif //DBNODES_DELETERELATIONBUTTONPROVIDER_H
