//
// Created by artem on 16.04.2021.
//

#ifndef DBNODES_MOVABLERECT_H
#define DBNODES_MOVABLERECT_H

#include "QMouseEvent"

#include "AbstractNode.h"

namespace DbNodes::Widgets::Minimap
{

    class MovableRect: public Abstract::AbstractNode
    {
        Q_OBJECT

        public:
            explicit MovableRect(const float &scaleCoefficient, QWidget *parent = nullptr);

            bool workingWithFocusAvailable();

            void calculateSize();

        private:
            bool enableWorkingWithFocus = true;

            const float &scaleCoefficient;

            void mouseMoveEvent(QMouseEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

        public: signals:
            void moveToPositionSignal(const QPoint &pos);
    };

}


#endif //DBNODES_MOVABLERECT_H
