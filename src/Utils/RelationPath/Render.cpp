//
// Created by artem on 12.04.2021.
//

#include "Render.h"

namespace DbNodes::Utils::RelationPath {

    /**
     * RelationPath constructor
     * @param parent
     */
    Render::Render(QWidget *parent): AbstractUtil(parent)
    {
        parentWidget = parent;
    }

    /**
     * Draw path for relation
     * @param painter (Default painter)
     * @param path (Type of drawing path)
     * @param startPoint (Start of drawing path)
     * @param endPoint (Target of drawing path)
     */
    void Render::drawPath(QPainter &painter, QPainterPath &path, const QPoint &startPoint, const QPoint &endPoint)
    {
        int cP12_x = endPoint.x() + (startPoint.x() - endPoint.x()) / 2;

        path.moveTo(endPoint.x(), endPoint.y());
        path.cubicTo(cP12_x, endPoint.y(), cP12_x, startPoint.y(), startPoint.x(), startPoint.y());

        painter.drawPath(path);
    }

    /**
     * Determinate position of relation widget
     * @param startParams (position and width of drawing table)
     * @param direction (direction of drawing start position)
     * @return data for correct drawing relation path
     */
    QPoint Render::calculateForWidget(
        Abstract::ParamsForDrawing &startParams,
        const Abstract::PathDirection &direction
    ) {

        QPoint &startPoint = startParams.first;

        QPoint movePoint;

        switch (direction) {
            case Abstract::leftToRight:
                movePoint = {
                    startPoint.x() + 2,
                    startPoint.y() - parentWidget->height() / 2
                };

                startPoint.rx() += parentWidget->width();

                break;

            case Abstract::rightToLeft:
                movePoint = {
                    startPoint.x() - parentWidget->width() - 2,
                    startPoint.y() - parentWidget->height() / 2
                };

                startPoint.rx() -= parentWidget->width() + 2;

                break;
        }

        return movePoint;
    }

    /**
     * Transform start and end positions of path depending on drawing direction
     * @param startParams (position and width of target index column)
     * @param endParams (position and width of related foreign column)
     * @param direction (drawing path direction)
     */
    void Render::calculatePosition(
        Abstract::ParamsForDrawing &startParams,
        Abstract::ParamsForDrawing &endParams,
        const Abstract::PathDirection &direction
    ) {
        switch (direction) {
            case Abstract::leftToRight:
                startParams.first.rx() += startParams.second;
                return;
            case Abstract::rightToLeft:
                endParams.first.rx() += endParams.second;
                return;
        }
    }

    /**
     * Determinate direction of drawing path
     * @param startPoint (point of related column)
     * @param endPoint (point of index column)
     * @return direction
     */
    Abstract::PathDirection Render::determinateDirection(const QPoint &startPoint, const QPoint &endPoint)
    {
        if (endPoint.x() < startPoint.x()) {
            return Abstract::PathDirection::rightToLeft;
        } else {
            return Abstract::PathDirection::leftToRight;
        }
    }
}