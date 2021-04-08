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

namespace DbNodes::Abstract {

    class AbstractRelationView: public QWidget
    {
        Q_OBJECT

        public:
            explicit AbstractRelationView(
                QWidget *parent,
                const COLUMN_POINTER &pkColumn,
                const COLUMN_POINTER &fkColumn
            );

            virtual void updateRelation(QPainter &painter, QPainterPath &path);
            virtual int getCurrentTypeId();

            virtual bool hasRelationPositionType();
            virtual int relationPositionType();
            virtual void setRelationPositionType(const int &type);

        protected:
            COLUMN_POINTER fkColumn;
            COLUMN_POINTER pkColumn;

            QMenu * createContextMenu();

        public: signals:
            void clickedDelete();
            void clickedGoToRelatedTable();
            void changedRelationType(const int &type);
    };

}


#endif //DBNODES_ABSTRACTRELATIONVIEW_H
