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
                const Nodes::Table::ColumnPrt &pkColumn,
                const Nodes::Table::ColumnPrt &fkColumn,
                const Dictionaries::RelationPositionsDictionary::Type &relationPosition,
                QWidget *parent = nullptr
            );

            void contextMenuEvent(QContextMenuEvent *event) override;
            void setSidePositionsName(const Dictionaries::RelationPositionsDictionary::Type &position);

            void updateRelation(QPainter &painter, QPainterPath &path) override;
            Dictionaries::RelationTypesDictionary::Type getCurrentTypeId() override;

            void switchPosition();

            bool hasRelationPositionType() override;

            Dictionaries::RelationPositionsDictionary::Type relationPositionType() override;

            void setRelationPositionType(
                const Dictionaries::RelationPositionsDictionary::Type &type
            ) override;

            static Dictionaries::RelationPositionsDictionary::Type getNextRelationPositionType(
                const Dictionaries::RelationPositionsDictionary::Type &currentRelationType
            );

        private:
            QString sidePositionName;
            QAction *switchSideAction{};

            Dictionaries::RelationPositionsDictionary::Type relationPosition{};

            void initUI();
    };

}

#endif //DBNODES_RELATIONLINK_H
