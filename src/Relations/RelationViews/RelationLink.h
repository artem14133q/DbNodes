//
// Created by artem on 14.02.2021.
//

#ifndef DBNODES_RELATIONLINK_H
#define DBNODES_RELATIONLINK_H

#include "QWidget"
#include "QPointer"
#include "QContextMenuEvent"

#include "Noderow.h"
#include "AbstractRelationView.h"

namespace DbNodes::Relations {

    class RelationLink: public Abstract::AbstractRelationView
    {
        Q_OBJECT

        public:
            explicit RelationLink(
                const NODE_RAW_POINTER &pkNodeRaw,
                const NODE_RAW_POINTER &fkNodeRaw,
                const int &relationPosition,
                QWidget *parent = nullptr
            );

            void contextMenuEvent(QContextMenuEvent *event) override;
            void setSidePositionsName(const int &position);

            void updateRelation(QPainter &painter, QPainterPath &path) override;
            int getCurrentTypeId() override;

            void switchPosition();

        private:
            QString sidePositionName;
            QAction *switchSideAction{};

            int relationPosition{};

            void initUI();
    };

}

#endif //DBNODES_RELATIONLINK_H
