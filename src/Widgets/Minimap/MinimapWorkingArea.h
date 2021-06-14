//
// Created by artem on 18.04.2021.
//

#ifndef DBNODES_MINIMAPWORKINGAREA_H
#define DBNODES_MINIMAPWORKINGAREA_H

#include "QWidget"
#include "QLabel"

#include "AbstractNode.h"
#include "MovableRect.h"

namespace DbNodes::Widgets::Minimap
{

    class MinimapWorkingArea: public QWidget
    {
        Q_OBJECT

        private:
            QColor selectedTablesColor;
            QColor focusedTablesColor;
            QColor tablesColor;

            const QList<Abstract::NodePtr> &nodes;
            const float &scaleCoefficient;

            QLabel *nodeTitle;

            void paintEvent(QPaintEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;

        public:
            MovableRect *movableRect{};

            void moveToPosition(const QPoint &pos);

            explicit MinimapWorkingArea(
                const QList<Abstract::NodePtr> &nodes,
                QLabel *nodeTitle,
                const float &scaleCoefficient,
                QWidget *parent = nullptr
            );

        public: signals:
            void moveToPositionSignal(const QPoint &pos);
    };

}

#endif //DBNODES_MINIMAPWORKINGAREA_H
