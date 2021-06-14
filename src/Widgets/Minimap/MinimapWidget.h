//
// Created by artem on 15.04.2021.
//

#ifndef DBNODES_MINIMAPWIDGET_H
#define DBNODES_MINIMAPWIDGET_H

#include "QPoint"
#include "QMouseEvent"
#include "QPaintEvent"
#include "QLabel"

#include "AbstractNode.h"
#include "MinimapWorkingArea.h"
#include "MinimapPositionsDictionary.h"

namespace DbNodes::Widgets::Minimap {

    class MinimapWidget: public Abstract::AbstractNode
    {
        Q_OBJECT

        private:
            const float ADD_WIDTH = 2;
            const float ADD_HEIGHT = 30;

            float scaleCoefficient = 0;
            float largeSize;

            QSize workAreaSize;

            MinimapWorkingArea *minimapWorkingArea{};
            Dictionaries::MinimapPositionsDictionary::Type position;

            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;

            void moveToPositionProxySignal(const QPoint &pos);

            void initUI(const QList<Abstract::NodePtr> &nodes);

            void calculateSize(int size);
            void calculateWorkingAreaSize();

        public:
            explicit MinimapWidget(
                const Dictionaries::MinimapPositionsDictionary::Type &position,
                const QList<Abstract::NodePtr> &nodes,
                const QSize &size, QWidget *parent = nullptr
            );

            void updatePos(QWidget *mappedWidget);

            int prepareSize(const int &size);

            void moveIndicator(const QPoint &pos);

            QPoint getIndicatorPos();

        public: signals:
            void moveToPositionSignal(const QPoint &pos);
    };

}


#endif //DBNODES_MINIMAPWIDGET_H
