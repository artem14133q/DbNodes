#ifndef DELETERELATIONBUTTON_H
#define DELETERELATIONBUTTON_H

#include "QPushButton"
#include "QWidget"
#include "QPointer"
#include "QContextMenuEvent"

#include "AbstractRelationView.h"
#include "Table/Column.h"

namespace DbNodes::Relations {

    class DeleteRelationButton : public Abstract::AbstractRelationView
    {
        Q_OBJECT

        public:
            explicit DeleteRelationButton(
                const COLUMN_POINTER &pkColumn,
                const COLUMN_POINTER &fkColumn,
                QWidget *parent = nullptr
            );

            void contextMenuEvent(QContextMenuEvent *event) override;

            void updateRelation(QPainter &painter, QPainterPath &path) override;
            int getCurrentTypeId() override;

        private:
            QColor color;

            void deleteRelation();

            void initUI();
    };


}

#endif // DELETERELATIONBUTTON_H
