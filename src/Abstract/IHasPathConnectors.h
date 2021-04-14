//
// Created by artem on 12.04.2021.
//

#ifndef DBNODES_IHASPATHCONNECTORS_H
#define DBNODES_IHASPATHCONNECTORS_H

namespace DbNodes::Abstract {

    enum PathDirection : int {
        leftToRight,
        rightToLeft,
    };

    typedef QPair<QPoint, int> ParamsForDrawing;

    class IHasPathConnectors {
        public:
            virtual ParamsForDrawing getDrawParams() = 0;
    };

}

#endif //DBNODES_IHASPATHCONNECTORS_H
