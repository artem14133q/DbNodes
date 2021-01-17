//
// Created by artem on 26.12.2020.
//

#ifndef DBNODES_STARTUPWIDGET_H
#define DBNODES_STARTUPWIDGET_H

#include "DrawableWidget.h"

namespace DbNodes::Widgets {

    class StartupWidget: public DbNodes::Abstract::DrawableWidget
    {
        public:
            explicit StartupWidget(QWidget *parent = nullptr);

        private:
            void initUi();
    };

}


#endif //DBNODES_STARTUPWIDGET_H
