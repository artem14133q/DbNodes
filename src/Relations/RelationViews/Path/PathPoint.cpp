//
// Created by artem on 08.04.2021.
//

#include "QHBoxLayout"
#include "QLabel"

#include "../helper.h"

#include "PathPoint.h"

namespace DbNodes::Relations::Path
{
    PathPoint::PathPoint(QWidget *parent) : AbstractNode(parent)
    {
        setFixedSize(20, 20);
        setObjectName("PathPoint");

        initUI();

        show();
    }

    void PathPoint::initUI()
    {
        setStyleSheet(Helper::getStyleFromFile("pathPoint"));

        auto layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0 ,0, 0);

        auto dot = new QLabel(this);
        dot->setFixedSize(10, 10);
        dot->setStyleSheet("background-color: #4285f4; border-radius: 5px;");

        layout->addStretch();
        layout->addWidget(dot);
        layout->addStretch();

        setLayout(layout);
    }

    Abstract::ParamsForDrawing PathPoint::getDrawParams()
    {
        return Abstract::ParamsForDrawing(
            {x(), y() + height() / 2},
            width()
        );
    }

    void PathPoint::contextMenuEvent(QContextMenuEvent *event)
    {
        auto *contextMenu = new QMenu();
        contextMenu->setStyleSheet(Helper::getStyleFromFile("tableMenu"));

        deleteNodeAction = contextMenu->addAction("Delete");

        connect(deleteNodeAction, &QAction::triggered, this, [this] {
            emit deleteNodeSignal();
        });

        auto menuPos = mapToGlobal(event->pos());
        menuPos.setX(menuPos.x() + 5);
        //Set visible menu
        contextMenu->exec(menuPos);
    }
}