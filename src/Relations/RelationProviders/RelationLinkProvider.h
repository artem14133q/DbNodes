//
// Created by artem on 28.02.2021.
//

#ifndef DBNODES_RELATIONLINKPROVIDER_H
#define DBNODES_RELATIONLINKPROVIDER_H

#include "RelationProvider.h"

namespace DbNodes::Relations {

    class RelationLinkProvider: public RelationProvider
    {
        public:
            Abstract::AbstractRelationView *create() override;
    };

}


#endif //DBNODES_RELATIONLINKPROVIDER_H
