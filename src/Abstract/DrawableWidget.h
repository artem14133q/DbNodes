//
// Created by artem on 19.12.2020.
//

#ifndef DBNODES_DRAWABLEWIDGET_H
#define DBNODES_DRAWABLEWIDGET_H

#include "QWidget"
#include "QPaintEvent"

namespace DbNodes::Abstract {

    class DrawableWidget : public QWidget
    {
        public:
            explicit DrawableWidget(QWidget *parent = nullptr);

        protected:
            void paintEvent(QPaintEvent *) override;
    };

}


#endif //DBNODES_DRAWABLEWIDGET_H
