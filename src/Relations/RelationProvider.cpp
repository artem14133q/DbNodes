//
// Created by artem on 28.02.2021.
//

#include "RelationProvider.h"

namespace DbNodes::Relations {

    RelationProvider::RelationProvider() = default;

    void RelationProvider::setParent(QWidget *parent)
    {
        parentWidget = parent;
    }

    QWidget *RelationProvider::getParentWidget()
    {
        return parentWidget;
    }

    void RelationProvider::setParameters(const QHash<QString, QVariant> &parameters)
    {
        parametersList = parameters;
    }

    Abstract::AbstractRelationView *RelationProvider::create()
    {
        return nullptr;
    }

    QVariant RelationProvider::getParameter(const QString &parameterName)
    {
        return parametersList.value(parameterName);
    }

    RelationProvider::~RelationProvider()
    {
        parametersList.clear();

        QObject::deleteLater();
    }

    Relation *RelationProvider::getRelation()
    {
        return relation;
    }

    void RelationProvider::setRelation(Relation *relation_)
    {
        relation = relation_;
    }

    void RelationProvider::initDefaultsConnects(Abstract::AbstractRelationView *relationView)
    {
        connect(
            relationView,
            &Abstract::AbstractRelationView::clickedDelete,
            getRelation(),
            &Relations::Relation::deleteRelation
        );

        connect(
            relationView,
            &Abstract::AbstractRelationView::changedRelationType,
            getRelation(),
            &Relations::Relation::enableRelationType
        );

        connect(
            relationView,
            &Abstract::AbstractRelationView::clickedGoToRelatedTable,
            getRelation(),
            &Relations::Relation::goToRelationTableSignal
        );
    }
};

