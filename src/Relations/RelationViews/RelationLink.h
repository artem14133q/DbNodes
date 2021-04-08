//
// Created by artem on 14.02.2021.
//

#ifndef DBNODES_RELATIONLINK_H
#define DBNODES_RELATIONLINK_H

#include "QWidget"
#include "QPointer"
#include "QContextMenuEvent"

#include "Table/Column.h"
#include "AbstractRelationView.h"

namespace DbNodes::Relations {

    class RelationLink: public Abstract::AbstractRelationView
    {
        Q_OBJECT

        public:
            explicit RelationLink(
                const COLUMN_POINTER &pkColumn,
                const COLUMN_POINTER &fkColumn,
                const int &relationPosition,
                QWidget *parent = nullptr
            );

            void contextMenuEvent(QContextMenuEvent *event) override;
            void setSidePositionsName(const int &position);

            void updateRelation(QPainter &painter, QPainterPath &path) override;
            int getCurrentTypeId() override;

            void switchPosition();

            bool hasRelationPositionType() override;
            int relationPositionType() override;
            void setRelationPositionType(const int &type) override;

            static int getNextRelationType(const int &currentRelationType);

        private:
            QString sidePositionName;
            QAction *switchSideAction{};

            int relationPosition{};

            void initUI();
    };

}

#endif //DBNODES_RELATIONLINK_H
