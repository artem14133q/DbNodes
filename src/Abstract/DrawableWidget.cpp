//
// Created by artem on 19.12.2020.
//

#include "DrawableWidget.h"
#include "QStyleOption"
#include "QPainter"
#include "QStyle"

namespace DbNodes::Abstract {

    DrawableWidget::DrawableWidget(QWidget *parent) : QWidget(parent) {}

    void DrawableWidget::paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        QStyleOption opt;
        opt.init(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

}