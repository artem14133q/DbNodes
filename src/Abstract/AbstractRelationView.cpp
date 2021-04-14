//
// Created by artem on 22.02.2021.
//

#include "AbstractRelationView.h"

#include <utility>
#include "../helper.h"

namespace DbNodes::Abstract {

    AbstractRelationView::AbstractRelationView(
        QWidget *parent,
        Nodes::Table::ColumnPrt pkColumn,
        Nodes::Table::ColumnPrt fkColumn
    ): QWidget(parent), fkColumn(std::move(fkColumn)), pkColumn(std::move(pkColumn)) {}

    QMenu * AbstractRelationView::createContextMenu()
    {
        auto *contextMenu = new QMenu();
        contextMenu->setStyleSheet(Helper::getStyleFromFile("tableMenu"));

        foreach (const int &relationTypeId, Dictionaries::RelationTypesDictionary::getDictionary().keys()) {

            auto type = (Dictionaries::RelationTypesDictionary::Type) relationTypeId;

            if (getCurrentTypeId() == type || type == Dictionaries::RelationTypesDictionary::Type::Undefined)
                continue;

            QAction* makeRelationPathType = contextMenu->addAction(
                "Show " + Dictionaries::RelationTypesDictionary::getValue(relationTypeId).toString()
            );

            connect(makeRelationPathType, &QAction::triggered, this, [this, type] {
                emit changedRelationType(type);
            });
        }

        QAction* goToRelatedTable = contextMenu->addAction("Go to related table");

        connect(goToRelatedTable, &QAction::triggered, this, [this] {
            emit clickedGoToRelatedTable();
        });

        return contextMenu;
    }

    void AbstractRelationView::updateRelation(QPainter &painter, QPainterPath &path) {}

    Dictionaries::RelationTypesDictionary::Type AbstractRelationView::getCurrentTypeId()
    {
        return Dictionaries::RelationTypesDictionary::Type::Undefined;
    }

    bool AbstractRelationView::hasRelationPositionType()
    {
        return false;
    }

    void AbstractRelationView::setRelationPositionType(
        const Dictionaries::RelationPositionsDictionary::Type &type
    ) {}

    Dictionaries::RelationPositionsDictionary::Type AbstractRelationView::relationPositionType()
    {
        return Dictionaries::RelationPositionsDictionary::Type::Undefined;
    }
}
