//
// Created by artem on 24.03.2021.
//

#include "QVBoxLayout"

#include "MainWidget.h"
#include "Hints.h"

namespace DbNodes::Widgets::StartupWidget {

    MainWidget::MainWidget(Saving::ProjectListFileResolver *projectsManager, QWidget *parent) : DrawableWidget(parent)
    {
        setObjectName("StartupWidget");

        initUI(projectsManager);
    }

    void MainWidget::initUI(Saving::ProjectListFileResolver *projectsManager)
    {
        auto *hl = new QHBoxLayout();

        auto *startupWidget = new Hints(this);

        if (!projectsManager->getProjectsMap().empty()) {
            auto *startupMenu = new Menu(projectsManager, this);

            connect(startupMenu, &Menu::openProjectSignal, this, [this] (const QString &path) {
                emit openProjectSignal(path);
            });

            connect(startupMenu, &Menu::updateMenuSignal, this, [this] {
                emit updateMenuSignal();
            });

            hl->addWidget(startupMenu);
        }

        auto *startupLayout = new QVBoxLayout();

        startupLayout->addStretch();
        startupLayout->addWidget(startupWidget);
        startupLayout->addStretch();

        hl->addStretch();
        hl->addLayout(startupLayout);
        hl->addStretch();

        setLayout(hl);
    }

}