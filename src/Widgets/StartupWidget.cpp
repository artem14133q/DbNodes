//
// Created by artem on 26.12.2020.
//

#include "StartupWidget.h"
#include "QVBoxLayout"
#include "QLabel"
#include "QStringList"
#include "../helper.h"

namespace DbNodes::Widgets {

    StartupWidget::StartupWidget(QWidget *parent): DbNodes::Abstract::DrawableWidget(parent)
    {
        setStyleSheet(Helper::getStyleFromFile("startupWidget"));

        initUi();
        adjustSize();

        show();
    }

    void StartupWidget::initUi()
    {
        auto* vb = new QVBoxLayout(this);

        auto content = Helper::getfileContent(":/content.startup").split("\n");

        foreach (QString line, content) {
            vb->addWidget(new QLabel(line, this));
        }

        setLayout(vb);
    }
}