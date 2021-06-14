#ifndef DELETERELATIONBUTTON_H
#define DELETERELATIONBUTTON_H

#include "QPushButton"
#include "QWidget"
#include "QPointer"
#include "QContextMenuEvent"

#include "AbstractRelationView.h"
#include "IHasPathConnectors.h"
#include "RelationPath/Render.h"
#include "Path/PathPoint.h"
#include "Table/Column.h"

namespace DbNodes::Relations {

    class DeleteRelationButton : public Abstract::AbstractRelationView
    {
        Q_OBJECT

        private:
            const int DELTA_X_WHEN_CREATE_POINT = 20;

            QColor color;

            Utils::RelationPath::Render *relationRenderer;

            Abstract::PathDirection direction;

            QList<Path::RelationPathPointPtr> points;

            void initUI();

        public:
            explicit DeleteRelationButton(
                const Nodes::Table::ColumnPrt &pkColumn,
                const Nodes::Table::ColumnPrt &fkColumn,
                QWidget *parent = nullptr
            );

            void contextMenuEvent(QContextMenuEvent *event) override;

            void updateRelation(QPainter &painter, QPainterPath &path) override;
            Dictionaries::RelationTypesDictionary::Type getCurrentTypeId() override;

            QList<Path::RelationPathPointPtr> getPoints();

            ~DeleteRelationButton() override;


        protected:
            void deleteRelation();

            void drawRelationPath(
                QPainter &painter,
                QPainterPath &path,
                Abstract::ParamsForDrawing &startParams,
                Abstract::ParamsForDrawing &endParams,
                const bool &moveEnable
            );

        public slots:
            Path::RelationPathPointPtr createPoint(const QPoint &pos = QPoint());

        public: signals:
            void createPathPointSignal(Path::PathPoint *point);
            void deletePathPointSignal(Path::PathPoint *point);
    };


}

#endif // DELETERELATIONBUTTON_H
