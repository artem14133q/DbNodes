//
// Created by artem on 05.03.2021.
//

#ifndef DBNODES_RELATIONFACTORY_H
#define DBNODES_RELATIONFACTORY_H

#include "AbstractRelationView.h"
#include "RelationProvider.h"
#include "Noderow.h"

namespace DbNodes::Relations {

    class RelationFactory
    {
        private:
            QHash<int, std::function< RelationProvider * ( void )> > callbacks;

        public:
            explicit RelationFactory();

            void registerProvider(const int &id, const std::function< RelationProvider * ( void )> &callback);

            Abstract::AbstractRelationView *resolveFactory(
                const int &id,
                const std::function< void ( RelationProvider * )> &callback
            );
    };

}

#endif //DBNODES_RELATIONFACTORY_H
