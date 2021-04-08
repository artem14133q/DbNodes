//
// Created by artem on 28.02.2021.
//

#include "QDebug"

#include "RelationLinkProvider.h"
#include "RelationViews/RelationLink.h"

namespace DbNodes::Relations {

    Abstract::AbstractRelationView *RelationLinkProvider::create()
    {
        auto *relationView = new RelationLink(getRelation()->getPkColumn(), getRelation()->getFkColumn(),
            getParameter("relationPosition").toInt(),
            getParentWidget()
        );

        initDefaultsConnects(relationView);

        return relationView;
    }
}