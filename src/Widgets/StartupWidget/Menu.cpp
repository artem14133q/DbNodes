//
// Created by artem on 21.03.2021.
//

#include "QScrollArea"
#include "QLabel"
#include "QPushButton"

#include "Project.h"
#include "Menu.h"
#include "SaveManager.h"
#include "../helper.h"

namespace DbNodes::Widgets::StartupWidget {

    Menu::Menu(Saving::ProjectListFileResolver *projectsManager, QWidget *parent)
        : DrawableWidget(parent), projectsManager(projectsManager)
    {
        setFixedWidth(700);
        setObjectName("StartupMenu");

        initUI();
    }

    void Menu::initUI()
    {
        setStyleSheet("QWidget#StartupMenu {border-right: 1px solid #595b5d;}");

        auto *layout = new QVBoxLayout();

        auto *titleLayout = new QHBoxLayout();

        auto *title = new QLabel("Projects", this);
        title->setStyleSheet(Helper::getStyleFromFile("title colorCorrector"));
        title->setContentsMargins(10, 0, 0, 10);

        titleLayout->addWidget(title);
        titleLayout->addStretch();

        auto *refresh = new QPushButton(this);
        refresh->setFixedSize(24, 24);
        refresh->setIcon(QIcon(Helper::getIconPath("refresh")));
        refresh->setStyleSheet(Helper::getStyleFromFile("buttonWithIcon"));
        connect(refresh, &QPushButton::clicked, this, &Menu::fillProjectsViewContainer);

        titleLayout->addWidget(refresh);
        titleLayout->addSpacing(43);

        layout->addLayout(titleLayout);

        auto *scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet("QScrollArea {border: 0;}");

        auto *verticalWidget = new QWidget(scrollArea);

        auto *verticalLayout = new QVBoxLayout();

        projectsViewContainer = new QVBoxLayout();

        fillProjectsViewContainer();

        verticalLayout->addLayout(projectsViewContainer);
        verticalLayout->addStretch();

        verticalWidget->setLayout(verticalLayout);
        scrollArea->setWidget(verticalWidget);
        layout->addWidget(scrollArea);

        setLayout(layout);

        verticalWidget->setFixedWidth(width() - 50);
    }

    Project *Menu::createProjectWidget(const QString &path, const QString &title)
    {
        auto *project = new Project(title, path, !Saving::SaveManager::fileExists(path),this);

        connect(project, &Project::deleteItSignal, this, &Menu::deleteProject);

        connect(project, &Project::clicked, this, [this, path, title] {
            emit openProjectSignal(path);
        });

        return project;
    }

    void Menu::deleteProject(const QString &path)
    {
        projects.value(path)->deleteLater();
        projects.remove(path);
        projectsManager->removeProject(path);
        update();
    }

    void Menu::fillProjectsViewContainer()
    {
        deleteProjects();

        foreach (const QString &path, projectsManager->getProjectsMap().keys()) {
            auto *projectWidget = createProjectWidget(path, projectsManager->getProjectsMap().value(path));

            projects.insert(path, projectWidget);
            projectsViewContainer->addWidget(projectWidget);
        }
    }

    void Menu::deleteProjects()
    {
        foreach (const QString &key, projects.keys()) {
            projects.value(key)->deleteLater();
        }

        projects.clear();
    }
}