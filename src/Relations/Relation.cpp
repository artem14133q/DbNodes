//
// Created by artem on 11.02.2021.
//

#include "QDebug"
#include "QTimer"

#include "Relation.h"
#include "RelationFactory.h"
#include "RelationProvider.h"

#include "../helper.h"

#include <utility>

namespace DbNodes::Relations {

    Relation::Relation(
            QWidget *parent,
            QString relationId,
            const int relationTypeId,
            COLUMN_POINTER &pkColumn,
            COLUMN_POINTER &fkColumn
    ): QObject(parent),
       parent(parent),
       relationId(std::move(relationId)),
       pkColumn(pkColumn),
       fkColumn(fkColumn)
    {
        enableRelationType(relationTypeId);
    }

    COLUMN_POINTER Relation::getPkColumn()
    {
        return pkColumn;
    }

    COLUMN_POINTER Relation::getFkColumn()
    {
        return fkColumn;
    }

    bool Relation::checkIsRelationValid()
    {
        return pkColumn && fkColumn;
    }

    Relation::~Relation()
    {
        delete relationView;

        parent->update();

        if (fkColumn) {
            fkColumn->disableFkRelationButton(false);
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
        emit goToRelatedTable(pkColumn->getTableId());
    }

    void Relation::raise()
    {
        relationView->raise();
    }

    int Relation::getRelationPositionType()
    {
        if (relationView->hasRelationPositionType()) {
            return relationView->relationPositionType();
        }

        return 0;
    }

    void Relation::setRelationPositionType(const int &type)
    {
        if (relationView->hasRelationPositionType()) {
            relationView->setRelationPositionType(type);
        }
    }
}
