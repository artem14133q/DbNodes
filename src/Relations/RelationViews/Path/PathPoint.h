//
// Created by artem on 08.04.2021.
//

#ifndef DBNODES_PATHPOINT_H
#define DBNODES_PATHPOINT_H

#include "AbstractNode.h"
#include "IHasPathConnectors.h"

typedef QPair<QPoint, QPoint> PointPair;

namespace DbNodes::Relations::Path {

    class PathPoint : public Abstract::AbstractNode, public Abstract::IHasPathConnectors
    {
        Q_OBJECT

        private:
            void initUI();

        public:
            enum CreatePathPointDirection {
                Left,
                Right
            };

            explicit PathPoint(QWidget *parent = nullptr);

            Abstract::ParamsForDrawing getDrawParams() override;

            ~PathPoint() override;

        protected:
            void contextMenuEvent(QContextMenuEvent *event) override;

        public: signals:
            void deleteRelationPathPointSignal();
    };

    typedef QPointer<Relations::Path::PathPoint> RelationPathPointPtr;
}

#endif //DBNODES_PATHPOINT_H
