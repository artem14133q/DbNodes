//
// Created by artem on 05.03.2021.
//

#include "RelationFactory.h"
#include "RelationTypesDictionary.h"
#include "RelationPositionsDictionary.h"

#include "RelationProviders/RelationLinkProvider.h"
#include "RelationProviders/DeleteRelationButtonProvider.h"

namespace DbNodes::Relations {

    RelationFactory::RelationFactory()
    {
        registerProvider(Dictionaries::RelationTypesDictionary::Type::Link, [] () -> RelationProvider * {
            auto *provider = new RelationLinkProvider();

            provider->setParameters({
                {"relationPosition", Dictionaries::RelationPositionsDictionary::Type::Left}
            });

            return provider;
        });

        registerProvider(Dictionaries::RelationTypesDictionary::Type::Path, [] () -> RelationProvider * {
            return new DeleteRelationButtonProvider();
        });
    }

    Abstract::AbstractRelationView *RelationFactory::resolveFactory(
        const Dictionaries::RelationTypesDictionary::Type &id,
        const std::function<void ( RelationProvider * )> &callback
    ) {
        RelationProvider *provider = callbacks.value(id)();

        if (callback != nullptr) {
            callback(provider);
        }

        Abstract::AbstractRelationView *view = provider->create();
        view->show();

        delete provider;

        return view;
    }

    void RelationFactory::registerProvider(
        const Dictionaries::RelationTypesDictionary::Type &id,
        const std::function< RelationProvider * ( void )> &callback
    ) {
        callbacks.insert(id, callback);
    }
}
