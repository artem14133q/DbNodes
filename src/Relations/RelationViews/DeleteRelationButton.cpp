#include "QMenu"

#include "DeleteRelationButton.h"
#include "RelationTypesDictionary.h"
#include "../../helper.h"

namespace DbNodes::Relations {

    DeleteRelationButton::DeleteRelationButton(
        const Nodes::Table::ColumnPrt &pkColumn,
        const Nodes::Table::ColumnPrt &fkColumn,
        QWidget *parent
    ):  Abstract::AbstractRelationView(parent, pkColumn, fkColumn) {
        setFixedSize(16, 16);
        setStyleSheet("QWidget{border: 0; border-radius: 8px;}");

        relationRenderer = new Utils::RelationPath::Render(this);

        color = QColor();
        color.setRgb(47, 167, 227);

        direction = Abstract::leftToRight;

        initUI();
    }

    void DeleteRelationButton::initUI()
    {
        auto delButton = new QPushButton("X", this);
        delButton->setFixedSize(16, 16);
        delButton->move(0, 0);
        delButton->setStyleSheet(Helper::getStyleFromFile("deleteArrowButton"));
        connect(delButton, &QPushButton::clicked, this, &DeleteRelationButton::deleteRelation);
    }

    void DeleteRelationButton::deleteRelation()
    {
        emit clickedDelete();
    }

    void DeleteRelationButton::contextMenuEvent(QContextMenuEvent *event)
    {
        auto contextMenu = createContextMenu();

        auto createPointAction = contextMenu->addAction("Create path point");

        connect(createPointAction, &QAction::triggered, this, [this] {
            createPoint();
        });

        auto menuPos = mapToGlobal(event->pos());
        menuPos.setX(menuPos.x() + 5);
        contextMenu->exec(menuPos);
    }

    void DeleteRelationButton::updateRelation(QPainter &painter, QPainterPath &path)
    {
        painter.setPen(QPen(color, 2, Qt::SolidLine, Qt::FlatCap));

        Abstract::ParamsForDrawing endParams;

        auto startParams = fkColumn->getDrawParams();

        foreach (const Path::RelationPathPointPtr &pointPtr, points) {
            endParams = pointPtr->getDrawParams();

            auto oldParams = endParams;

            drawRelationPath(painter, path, startParams, endParams, pointPtr == points.first());

            startParams = oldParams;
        }

        endParams = pkColumn->getDrawParams();

        drawRelationPath(painter, path, startParams, endParams, points.isEmpty());
    }

    Dictionaries::RelationTypesDictionary::Type DeleteRelationButton::getCurrentTypeId()
    {
        return Dictionaries::RelationTypesDictionary::Type::Path;
    }

    Path::RelationPathPointPtr DeleteRelationButton::createPoint(const QPoint &pos)
    {
        Helper::removeDeletedItems<Path::PathPoint>(points);

        auto point = new Path::PathPoint(parentWidget());

        connect(point, &Abstract::AbstractNode::deleteNodeSignal, this, [this, point] {
            points.removeAll(point);
            point->deleteLater();
            parentWidget()->update();
        });

        if (pos.isNull()) {
            int deltaX = 0;

            switch (direction) {
                case Abstract::leftToRight:
                    deltaX += (DELTA_X_WHEN_CREATE_POINT + width()); break;
                case Abstract::rightToLeft:
                    deltaX -= (DELTA_X_WHEN_CREATE_POINT + point->width()); break;
            }

            point->move(x() + deltaX, y());
        } else {
            point->move(pos);
        }

        points.push_front(Path::RelationPathPointPtr(point));

        emit createPathPointSignal(point);

        parentWidget()->update();

        return point;
    }

    DeleteRelationButton::~DeleteRelationButton()
    {
        delete relationRenderer;

        foreach (const Path::RelationPathPointPtr &point, points) {
            point->deleteLater();
        }

        points.clear();

        deleteLater();
    }

    void DeleteRelationButton::drawRelationPath(
        QPainter &painter,
        QPainterPath &path,
        Abstract::ParamsForDrawing &startParams,
        Abstract::ParamsForDrawing &endParams,
        const bool &moveEnable
    ) {
        auto currentDirection = relationRenderer->determinateDirection(startParams.first, endParams.first);

        relationRenderer->calculatePosition(startParams, endParams, currentDirection);

        if (moveEnable) {
            move(relationRenderer->calculateForWidget(startParams, currentDirection));
            direction = currentDirection;
        }

        relationRenderer->drawPath(painter, path, startParams.first, endParams.first);
    }

    QList<Path::RelationPathPointPtr> DeleteRelationButton::getPoints()
    {
        Helper::removeDeletedItems<Path::PathPoint>(points);

        return points;
    }
}
