//
// Created by artem on 12.04.2021.
//

#include "Render.h"

namespace DbNodes::Utils::RelationPath {

    Render::Render(QWidget *parent): QObject(parent)
    {
        parentWidget = parent;
    }

    void Render::drawPath(QPainter &painter, QPainterPath &path, const QPoint &startPoint, const QPoint &endPoint)
    {
        int cP12_x = endPoint.x() + (startPoint.x() - endPoint.x()) / 2;

        path.moveTo(endPoint.x(), endPoint.y());
        path.cubicTo(cP12_x, endPoint.y(), cP12_x, startPoint.y(), startPoint.x(), startPoint.y());

        painter.drawPath(path);
    }

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

    Abstract::PathDirection Render::determinateDirection(const QPoint &startPoint, const QPoint &endPoint)
    {
        if (endPoint.x() < startPoint.x()) {
            return Abstract::PathDirection::rightToLeft;
        } else {
            return Abstract::PathDirection::leftToRight;
        }
    }
}