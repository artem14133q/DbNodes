//
// Created by artem on 11.02.2021.
//

#include "QDebug"
#include "QTimer"

#include "Relation.h"
#include "RelationTypesDictionary.h"
#include "RelationFactory.h"
#include "RelationProvider.h"

#include "../helper.h"

#include <utility>

namespace DbNodes::Relations {

    Relation::Relation(
        QWidget *parent,
        QString relationId,
        const int relationTypeId,
        NODE_RAW_POINTER &pkNodeRaw,
        NODE_RAW_POINTER &fkNodeRaw
    ):  QObject(parent),
        parent(parent),
        relationId(std::move(relationId)),
        pkNodeRaw(pkNodeRaw),
        fkNodeRaw(fkNodeRaw)
    {
        enableRelationType(relationTypeId);
    }

    NODE_RAW_POINTER Relation::getPkNodeRaw()
    {
        return pkNodeRaw;
    }

    NODE_RAW_POINTER Relation::getFkNodeRaw()
    {
        return fkNodeRaw;
    }

    bool Relation::checkIsRelationValid()
    {
        return pkNodeRaw && fkNodeRaw;
    }

    Relation::~Relation()
    {
        delete relationView;

        parent->update();

        if (fkNodeRaw) {
            fkNodeRaw->disableFkRelationButton(false);
        }

        deleteLater();
    }

    const QString &Relation::getRelationId()
    {
        return relationId;
    }

    int Relation::getRelationTypeId() const
    {
        return relationType;
    }

    void Relation::updateRelation(QPainter &painter, QPainterPath &path)
    {
        relationView->updateRelation(painter, path);
    }

    void Relation::deleteRelation()
    {
        delete this;
    }

    void Relation::enableRelationType(const int &relationTypeId)
    {
        relationType = relationTypeId;

        delete relationView;

        parent->update();

        RelationFactory relationFactory;

        relationView = relationFactory.resolveFactory(
            relationTypeId,
            [this] (RelationProvider * provider) {
                provider->setParent(parent);
                provider->setRelation(this);
            }
        );
    }

    void Relation::goToRelationTableSignal()
    {
        emit goToRelatedTable(pkNodeRaw->getTableId());
    }

    void Relation::raise()
    {
        relationView->raise();
    }
}
