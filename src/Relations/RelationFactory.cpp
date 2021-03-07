//
// Created by artem on 05.03.2021.
//

#include "RelationFactory.h"
#include "RelationTypesDictionary.h"
#include "RelationLinkPositionsDictionary.h"

#include "RelationProviders/RelationLinkProvider.h"
#include "RelationProviders/DeleteRelationButtonProvider.h"

namespace DbNodes::Relations {

    RelationFactory::RelationFactory()
    {
        registerProvider(RELATION_TYPE_LINK, [] () -> RelationProvider * {
            auto *provider = new RelationLinkProvider();

            provider->setParameters({
                {"relationPosition", RELATION_LINK_POSITION_LEFT}
            });

            return provider;
        });

        registerProvider(RELATION_TYPE_PATH, [] () -> RelationProvider * {
            return new DeleteRelationButtonProvider();
        });
    }

    Abstract::AbstractRelationView *RelationFactory::resolveFactory(
        const int &id,
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

    void RelationFactory::registerProvider(const int &id, const std::function< RelationProvider * ( void )> &callback)
    {
        callbacks.insert(id, callback);
    }
}
