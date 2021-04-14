//
// Created by artem on 22.02.2021.
//

#ifndef DBNODES_ABSTRACTRELATIONVIEW_H
#define DBNODES_ABSTRACTRELATIONVIEW_H

#include "QPointer"
#include "QMenu"
#include "QPainter"
#include "QPainterPath"

#include "DrawableWidget.h"
#include "Table/Column.h"

#include "RelationTypesDictionary.h"
#include "RelationPositionsDictionary.h"

namespace DbNodes::Abstract {

    class AbstractRelationView: public QWidget
    {
        Q_OBJECT

        public:
            explicit AbstractRelationView(
                QWidget *parent,
                Nodes::Table::ColumnPrt pkColumn,
                Nodes::Table::ColumnPrt fkColumn
            );

            virtual void updateRelation(QPainter &painter, QPainterPath &path);
            virtual Dictionaries::RelationTypesDictionary::Type getCurrentTypeId();

            virtual bool hasRelationPositionType();

            virtual Dictionaries::RelationPositionsDictionary::Type relationPositionType();

            virtual void setRelationPositionType(
                const Dictionaries::RelationPositionsDictionary::Type &type
            );

        protected:
            Nodes::Table::ColumnPrt fkColumn;
            Nodes::Table::ColumnPrt pkColumn;

            QMenu * createContextMenu();

        public: signals:
            void clickedDelete();
            void clickedGoToRelatedTable();
            void changedRelationType(const Dictionaries::RelationTypesDictionary::Type &type);
    };

}


#endif //DBNODES_ABSTRACTRELATIONVIEW_H
