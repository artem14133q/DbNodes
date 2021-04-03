//
// Created by artem on 21.03.2021.
//

#include "QHBoxLayout"
#include "QLabel"

#include "Project.h"

#include <utility>
#include "../helper.h"

namespace DbNodes::Widgets::StartupWidget {

    Project::Project(QString projectName, QString path, const bool &deleted, QWidget *parent)
        : DrawableWidget(parent), projectName(std::move(projectName)), path(std::move(path)), deleted(deleted)
    {
        setFixedHeight(80);
        setObjectName("Project");

        initUi();
    }

    void Project::initUi()
    {
        setStyleSheet(Helper::getStyleFromFile("startupMenuItem"));

        setProperty("disabled", deleted);

        auto *layout = new QHBoxLayout();

        auto *namePathLayout = new QVBoxLayout();

        auto *nameLayout = new QHBoxLayout();

        if (deleted) {
            auto *danger = new QLabel(this);
            danger->setPixmap(QPixmap(Helper::getIconPath("danger")));
            danger->setToolTip("The file has been deleted or moved");

            nameLayout->addWidget(danger);
        }

        auto *projectNameLabel = new QLabel(projectName, this);
        projectNameLabel->setStyleSheet(Helper::getStyleFromFile("title colorCorrector"));

        nameLayout->addWidget(projectNameLabel);

        nameLayout->addStretch();

        auto *pathLabel = new QLabel(Helper::replaceHomePathInFullPath(path), this);
        pathLabel->setStyleSheet(Helper::getStyleFromFile("subtitle"));

        namePathLayout->addLayout(nameLayout);
        namePathLayout->addWidget(pathLabel);

        layout->addLayout(namePathLayout);

        layout->addStretch();

        auto *optionButton = new QToolButton(this);
        optionButton->setMenu(createToolMenu(optionButton));
        optionButton->setPopupMode(QToolButton::InstantPopup);
        optionButton->setIcon(QIcon(Helper::getIconPath("settings")));
        optionButton->setStyleSheet(Helper::getStyleFromFile("buttonWithIcon"));
        optionButton->setFixedSize(24, 24);
        layout->addWidget(optionButton);

        setLayout(layout);
    }

    void Project::mousePressEvent(QMouseEvent *event)
    {
        if (!deleted) {
            emit clicked();
        }
    }

    QMenu *Project::createToolMenu(QToolButton *button)
    {
        auto menu = new QMenu(button);
        menu->setStyleSheet(Helper::getStyleFromFile("outline"));

        auto *removeAction =  menu->addAction("Remove from list");

        connect(removeAction, &QAction::triggered, this, [this] {
            emit deleteItSignal(path);
        });

        return  menu;
    }

}