//
// Created by artem on 15.04.2021.
//

#include "QStyleOption"
#include "QDebug"
#include "QCursor"
#include "QVBoxLayout"
#include "QTimer"

#include "MinimapWidget.h"
#include "MinimapSettings.h"
#include "TableNode.h"
#include "config.h"

namespace DbNodes::Widgets::Minimap{

    MinimapWidget::MinimapWidget(
        const Dictionaries::MinimapPositionsDictionary::Type &position,
        const QList<Abstract::NodePtr> &nodes,
        const QSize &workAreaSize,
        QWidget *parent
    ): Abstract::AbstractNode(parent), workAreaSize(workAreaSize), position(position)
    {
        setObjectName("MinimapWidget");

        Helper::subscribeSettingUpdate("minimap.position", [this] (const QVariant &value) {
            this->position = (Dictionaries::MinimapPositionsDictionary::Type) value.toInt();
            update();
        });

        // Choose large value, for valid scaling
        largeSize = (workAreaSize.width() > workAreaSize.height())
                ? (float) workAreaSize.width()
                : (float) workAreaSize.height();

        Helper::subscribeSettingUpdate("minimap.size",[this] (const QVariant &value) {
            calculateSize(value.toInt());
            calculateWorkingAreaSize();
            update();
        });

        calculateSize(Helper::getSettingValue("minimap.size").toInt());

        initUI(nodes);

        calculateWorkingAreaSize();

        QTimer::singleShot(500, [this] {
            minimapWorkingArea->movableRect->calculateSize();
        });

        show();
    }

    void MinimapWidget::initUI(const QList<Abstract::NodePtr> &nodes)
    {
        setStyleSheet(Helper::getStyleFromFile("minimapWidget"));

        auto vl = new QVBoxLayout();
        vl->setSpacing(1);
        vl->setContentsMargins(1, 1, 1, 1);

        auto headLayout = new QHBoxLayout();
        headLayout->addSpacing(5);

        auto nodeNameLabel = new QLabel(this);
        nodeNameLabel->setStyleSheet(Helper::getStyleFromFile("nodeNameLabel"));
        nodeNameLabel->setFixedHeight(24);
        headLayout->addWidget(nodeNameLabel);

        headLayout->addStretch();

        auto optionButton = new QPushButton(this);
        optionButton->setFixedSize(24, 24);
        optionButton->setStyleSheet(Helper::getStyleFromFile("buttonWithIcon"));
        optionButton->setIcon(QIcon(Helper::getIconPath("settings")));

        connect(optionButton, &QPushButton::clicked, this, [this] {
            new Modals::MinimapSettings(this);
        });

        headLayout->addWidget(optionButton);

        vl->addLayout(headLayout);

        minimapWorkingArea = new MinimapWorkingArea(nodes, nodeNameLabel, scaleCoefficient, this);

        connect(
            minimapWorkingArea,
            &MinimapWorkingArea::moveToPositionSignal,
            this,
            &MinimapWidget::moveToPositionProxySignal
        );

        vl->addWidget(minimapWorkingArea);

        setLayout(vl);
    }

    void MinimapWidget::moveIndicator(const QPoint &pos)
    {
        if (underMouse()) return;

        minimapWorkingArea->movableRect->restrictedMove(pos.x(), pos.y());
    }

    QPoint MinimapWidget::getIndicatorPos()
    {
        return minimapWorkingArea->movableRect->pos();
    }

    void MinimapWidget::mousePressEvent(QMouseEvent *event)
    {
        event->ignore();
    }

    void MinimapWidget::moveToPositionProxySignal(const QPoint &pos)
    {
        emit moveToPositionSignal(pos);
    }

    int MinimapWidget::prepareSize(const int &size)
    {
        return size / scaleCoefficient;
    }

    void MinimapWidget::mouseMoveEvent(QMouseEvent *event)
    {
        event->ignore();
    }

    void MinimapWidget::updatePos(QWidget *mappedWidget)
    {
        using Dictionaries::MinimapPositionsDictionary;

        int newX = 0;
        int newY = 0;

        if (position & MinimapPositionsDictionary::Top) {
            newY = 20;
        }

        if (position & MinimapPositionsDictionary::Bottom) {
            newY = mappedWidget->height() - height() - 20;
        }

        if (position & MinimapPositionsDictionary::Left) {
            newX = 20;
        }

        if (position & MinimapPositionsDictionary::Right) {
            newX = mappedWidget->width() - width() - 20;
        }

        move(newX, newY);
    }

    void MinimapWidget::calculateSize(int size)
    {
        scaleCoefficient = largeSize / size;

        setFixedSize(
            workAreaSize.width() / scaleCoefficient + ADD_WIDTH,
            workAreaSize.height() / scaleCoefficient + ADD_HEIGHT
        );
    }

    void MinimapWidget::calculateWorkingAreaSize()
    {
        minimapWorkingArea->setFixedSize(
            workAreaSize.width() / scaleCoefficient,
            workAreaSize.height() / scaleCoefficient
        );

        minimapWorkingArea->movableRect->move(0 ,0);
        minimapWorkingArea->movableRect->calculateSize();
    }
}