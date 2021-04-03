//
// Created by artem on 26.12.2020.
//

#ifndef DBNODES_HINTS_H
#define DBNODES_HINTS_H

#include "DrawableWidget.h"

namespace DbNodes::Widgets::StartupWidget {

    class Hints: public DbNodes::Abstract::DrawableWidget
    {
        public:
            explicit Hints(QWidget *parent = nullptr);

        private:
            void initUi();
    };

}


#endif //DBNODES_HINTS_H
