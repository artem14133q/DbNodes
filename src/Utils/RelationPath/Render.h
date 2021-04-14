//
// Created by artem on 12.04.2021.
//

#ifndef DBNODES_RENDER_H
#define DBNODES_RENDER_H

#include "QWidget"
#include "QPainter"
#include "QPainterPath"

#include "IHasPathConnectors.h"

namespace DbNodes::Utils::RelationPath {

    class Render : public QObject
    {
        private:
            QWidget *parentWidget;

        public:
            explicit Render(QWidget *parent);

            void drawPath(
                QPainter &painter,
                QPainterPath &path,
                const QPoint &startPoint,
                const QPoint &endPoint
            );

            void calculatePosition(
                Abstract::ParamsForDrawing &startParams,
                Abstract::ParamsForDrawing &endParams,
                const Abstract::PathDirection &direction
            );

            QPoint calculateForWidget(
                Abstract::ParamsForDrawing &startParams,
                const Abstract::PathDirection &direction
            );

            Abstract::PathDirection determinateDirection(const QPoint &startPoint, const QPoint &endPoint);
    };

}

#endif //DBNODES_RENDER_H
