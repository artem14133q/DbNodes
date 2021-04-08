//
// Created by artem on 22.02.2021.
//

#include "AbstractRelationView.h"
#include "RelationTypesDictionary.h"
#include "../helper.h"

namespace DbNodes::Abstract {

    AbstractRelationView::AbstractRelationView(
        QWidget *parent,
        const COLUMN_POINTER &pkColumn,
        const COLUMN_POINTER &fkColumn
    ): QWidget(parent), fkColumn(fkColumn), pkColumn(pkColumn) {}

    QMenu * AbstractRelationView::createContextMenu()
    {
        auto *contextMenu = new QMenu();
        contextMenu->setStyleSheet(Helper::getStyleFromFile("tableMenu"));

        foreach (const int &relationTypeId, Dictionaries::RelationTypesDictionary::getDictionary().keys()) {

            if (getCurrentTypeId() == relationTypeId) continue;

            QAction* makeRelationPathType = contextMenu->addAction(
                "Show " + Dictionaries::RelationTypesDictionary::getValue(relationTypeId).toString()
            );

            connect(makeRelationPathType, &QAction::triggered, this, [this, relationTypeId] {
                emit changedRelationType(relationTypeId);
            });
        }

        QAction* goToRelatedTable = contextMenu->addAction("Go to related table");

        connect(goToRelatedTable, &QAction::triggered, this, [this] {
            emit clickedGoToRelatedTable();
        });

        return contextMenu;
    }

    void AbstractRelationView::updateRelation(QPainter &painter, QPainterPath &path) {}

    int AbstractRelationView::getCurrentTypeId() { return 0; }

    bool AbstractRelationView::hasRelationPositionType()
    {
        return false;
    }

    int AbstractRelationView::relationPositionType()
    {
        return 0;
    }

    void AbstractRelationView::setRelationPositionType(const int &type) {}
}
