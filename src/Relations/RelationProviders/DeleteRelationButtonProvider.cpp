//
// Created by artem on 03.03.2021.
//

#include "DeleteRelationButtonProvider.h"
#include "RelationViews/DeleteRelationButton.h"

namespace DbNodes::Relations {

    Abstract::AbstractRelationView *DeleteRelationButtonProvider::create()
    {
        auto *relationView = new DeleteRelationButton(
            getRelation()->getPkNodeRaw(),
            getRelation()->getFkNodeRaw(),
            getParentWidget()
        );

        initDefaultsConnects(relationView);

        return relationView;
    }

}