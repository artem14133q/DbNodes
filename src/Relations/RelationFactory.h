//
// Created by artem on 05.03.2021.
//

#ifndef DBNODES_RELATIONFACTORY_H
#define DBNODES_RELATIONFACTORY_H

#include "RelationTypesDictionary.h"
#include "AbstractRelationView.h"
#include "RelationProvider.h"
#include "Table/Column.h"

namespace DbNodes::Relations {

    class RelationFactory
    {
        private:
            QHash<Dictionaries::RelationTypesDictionary::Type, std::function< RelationProvider * ( void )> > callbacks;

        public:
            explicit RelationFactory();

            void registerProvider(
                const Dictionaries::RelationTypesDictionary::Type &id,
                const std::function< RelationProvider * ( void )> &callback
            );

            Abstract::AbstractRelationView *resolveFactory(
                const Dictionaries::RelationTypesDictionary::Type &id,
                const std::function< void ( RelationProvider * )> &callback
            );
    };

}

#endif //DBNODES_RELATIONFACTORY_H
