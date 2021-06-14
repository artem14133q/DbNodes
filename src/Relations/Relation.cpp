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
        const Dictionaries::RelationTypesDictionary::Type relationTypeId,
        Nodes::Table::ColumnPrt &pkColumn,
        Nodes::Table::ColumnPrt &fkColumn
    ): QObject(parent),
       parent(parent),
       relationId(std::move(relationId)),
       pkColumn(pkColumn),
       fkColumn(fkColumn)
    {
        enableRelationType(relationTypeId);
    }

    Nodes::Table::ColumnPrt Relation::getPkColumn()
    {
        return pkColumn;
    }

    Nodes::Table::ColumnPrt Relation::getFkColumn()
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

    void Relation::enableRelationType(const Dictionaries::RelationTypesDictionary::Type &relationTypeId)
    {
        relationType = relationTypeId;

        delete relationView;

        parent->update();

        RelationFactory relationFactory;

        relationView = relationFactory.resolveFactory(relationTypeId,[this] (RelationProvider * provider) {
            provider->setParent(parent);
            provider->setRelation(this);
        });
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

    void Relation::setRelationPositionType(const Dictionaries::RelationPositionsDictionary::Type &type)
    {
        if (relationView->hasRelationPositionType()) {
            relationView->setRelationPositionType(type);
        }
    }

    void Relation::createNodeInWorkAreaProxy(Abstract::AbstractNode *node)
    {
        emit createNodeInWorkAreaSignal(node);
    }

    void Relation::deleteNodeInWorkAreaProxy(Abstract::AbstractNode *node)
    {
        emit deleteNodeInWorkAreaSignal(node);
    }

    Abstract::AbstractRelationView *Relation::getAbstractRelationView()
    {
        return relationView;
    }
}
