//
// Created by artem on 14.02.2021.
//

#include "QLabel"
#include "QHBoxLayout"
#include "QPushButton"
#include "QMenu"
#include "QDebug"

#include "RelationLink.h"
#include "Workarea.h"
#include "../../helper.h"

namespace DbNodes::Relations {

    RelationLink::RelationLink(
        const Nodes::Table::ColumnPrt &pkColumn,
        const Nodes::Table::ColumnPrt &fkColumn,
        const Dictionaries::RelationPositionsDictionary::Type &relationPosition,
        QWidget *parent
    ): Abstract::AbstractRelationView(parent, pkColumn, fkColumn), relationPosition(relationPosition) {
        setObjectName("RelationLink");
        setFixedHeight(26);
        setStyleSheet(Helper::getStyleFromFile("relationLink"));
        initUI();
        setSidePositionsName(getNextRelationPositionType(relationPosition));
        adjustSize();
    }

    void RelationLink::initUI()
    {
        auto *layout = new QHBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->setContentsMargins(2,2,2,2);

        auto *label = new QLabel("  " + pkColumn->getTableName() + " ", this);
        label->setFixedHeight(22);
        label->setStyleSheet(Helper::getStyleFromFile("relationLinkLabel"));
        layout->addWidget(label);

        auto *closeBtn = new QPushButton(" X ", this);
        closeBtn->setFixedWidth(22);
        closeBtn->setFixedHeight(22);
        closeBtn->setStyleSheet(Helper::getStyleFromFile("relationLinkDeleteButton"));

        connect(closeBtn, &QPushButton::clicked, this, [this] () {
            emit clickedDelete();
        });

        layout->addWidget(closeBtn);

        setLayout(layout);
    }

    void RelationLink::contextMenuEvent(QContextMenuEvent *event)
    {
        auto *contextMenu = createContextMenu();

        switchSideAction = contextMenu->addAction(sidePositionName);

        connect(switchSideAction, &QAction::triggered, this, &RelationLink::switchPosition);

        auto menuPos = mapToGlobal(event->pos());
        menuPos.setX(menuPos.x() + 5);
        //Set visible menu
        contextMenu->exec(menuPos);
    }

    void RelationLink::setSidePositionsName(
        const Dictionaries::RelationPositionsDictionary::Type &position
    ) {
        sidePositionName = QString("Move to the %1 side")
                .arg(Dictionaries::RelationPositionsDictionary::getValue(position).toString());
    }

    void RelationLink::updateRelation(QPainter &, QPainterPath &)
    {
        auto fkParams = fkColumn->getDrawParams();
        auto fkPos = fkParams.first;

        switch (relationPosition) {
            case Dictionaries::RelationPositionsDictionary::Type::Left :
                move(
                    fkPos.x() - width() - 2,
                    fkPos.y() - height() / 2
                );

                break;
            case Dictionaries::RelationPositionsDictionary::Type::Right :
                move(
                    fkPos.x() + fkParams.second + 2,
                    fkPos.y() - height() / 2
                );

                break;

            case Dictionaries::RelationPositionsDictionary::Undefined:
                break;
        }
    }

    void RelationLink::switchPosition()
    {
        setSidePositionsName(relationPosition);
        relationPosition = getNextRelationPositionType(relationPosition);

        parentWidget()->update();
    }

    Dictionaries::RelationTypesDictionary::Type RelationLink::getCurrentTypeId()
    {
        return Dictionaries::RelationTypesDictionary::Type::Link;
    }

    bool RelationLink::hasRelationPositionType()
    {
        return true;
    }

    Dictionaries::RelationPositionsDictionary::Type RelationLink::relationPositionType()
    {
        return relationPosition;
    }

    void RelationLink::setRelationPositionType(
        const Dictionaries::RelationPositionsDictionary::Type &currentRelationType
    ) {
        relationPosition = currentRelationType;
        setSidePositionsName(getNextRelationPositionType(relationPosition));
    }

    Dictionaries::RelationPositionsDictionary::Type RelationLink::getNextRelationPositionType(
            const Dictionaries::RelationPositionsDictionary::Type &currentRelationType
    ) {
        switch (currentRelationType) {
            case Dictionaries::RelationPositionsDictionary::Type::Left :
                return Dictionaries::RelationPositionsDictionary::Type::Right;

            case Dictionaries::RelationPositionsDictionary::Type::Right :
                return Dictionaries::RelationPositionsDictionary::Type::Left;

            default:
                return Dictionaries::RelationPositionsDictionary::Type::Undefined;
        }
    }
}
