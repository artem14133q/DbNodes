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
#include "RelationTypesDictionary.h"
#include "RelationLinkPositionsDictionary.h"
#include "../../helper.h"

namespace DbNodes::Relations {

    RelationLink::RelationLink(
        const NODE_RAW_POINTER &pkNodeRaw,
        const NODE_RAW_POINTER &fkNodeRaw,
        const int &relationPosition,
        QWidget *parent
    ):  Abstract::AbstractRelationView(parent, pkNodeRaw, fkNodeRaw), relationPosition(relationPosition) {
        setObjectName("RelationLink");
        setFixedHeight(26);
        setStyleSheet(Helper::getStyleFromFile("relationLink"));
        initUI();
        setSidePositionsName(getNextRelationType(relationPosition));
        adjustSize();
    }

    void RelationLink::initUI()
    {
        auto *layout = new QHBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->setContentsMargins(2,2,2,2);

        auto *label = new QLabel("  " + pkNodeRaw->getTableName() + " ", this);
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

    void RelationLink::setSidePositionsName(const int &position)
    {
        sidePositionName = QString("Move to the %1 side")
                .arg(Dictionaries::RelationLinkPositionsDictionary::getValue(position).toString());
    }

    void RelationLink::updateRelation(QPainter &, QPainterPath &)
    {
        int *fkBuf = fkNodeRaw->dataForPaint();

        switch (relationPosition) {
            case RELATION_LINK_POSITION_LEFT:
                move(
                        fkBuf[0] - width() - 2,
                        fkBuf[1] - height() / 2
                );

                break;
            case RELATION_LINK_POSITION_RIGHT:
                move(
                        fkBuf[0] + fkBuf[2] + 2,
                        fkBuf[1] - height() / 2
                );

                break;
        }
    }

    void RelationLink::switchPosition()
    {
        setSidePositionsName(relationPosition);
        relationPosition = getNextRelationType(relationPosition);
    }

    int RelationLink::getCurrentTypeId()
    {
        return RELATION_TYPE_LINK;
    }

    bool RelationLink::hasRelationPositionType()
    {
        return true;
    }

    int RelationLink::relationPositionType()
    {
        return relationPosition;
    }

    void RelationLink::setRelationPositionType(const int &type)
    {
        relationPosition = type;
        setSidePositionsName(getNextRelationType(relationPosition));
    }

    int RelationLink::getNextRelationType(const int &currentRelationType)
    {
        switch (currentRelationType) {
            case RELATION_LINK_POSITION_LEFT:
                return RELATION_LINK_POSITION_RIGHT;

            case RELATION_LINK_POSITION_RIGHT:
                return RELATION_LINK_POSITION_LEFT;

            default:
                return 0;
        }
    }
}
