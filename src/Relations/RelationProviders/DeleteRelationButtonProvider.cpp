//
// Created by artem on 03.03.2021.
//

#include "DeleteRelationButtonProvider.h"
#include "RelationViews/DeleteRelationButton.h"

namespace DbNodes::Relations {

    Abstract::AbstractRelationView *DeleteRelationButtonProvider::create()
    {
        auto *relationView = new DeleteRelationButton(
            getRelation()->getPkColumn(),
            getRelation()->getFkColumn(),
            getParentWidget()
        );

        initDefaultsConnects(relationView);

        connect(
            relationView,
            &DeleteRelationButton::createPathPointSignal,
            getRelation(),
            &Relation::createNodeInWorkAreaProxy
        );

        connect(
            relationView,
            &DeleteRelationButton::deletePathPointSignal,
            getRelation(),
            &Relation::deleteNodeInWorkAreaProxy
        );

        return relationView;
    }

}