//
// Created by artem on 05.04.2021.
//

#include "RecentMenu.h"
#include "../helper.h"

namespace DbNodes::Widgets::Menus {

    RecentMenu::RecentMenu(Saving::ProjectListFileResolver *resolver, QWidget *parent)
        : QMenu(parent), resolver(resolver)
    {
        setTitle("Open Recent");

        generateMenu();
    }

    void RecentMenu::updateMenu()
    {
        foreach (auto action, actionList) {
            delete action;
        }

        actionList.clear();

        generateMenu();
    }

    void RecentMenu::generateMenu()
    {
        if (resolver->getProjectsMap().keys().isEmpty()) {
            auto action = addAction("empty");
            action->setDisabled(true);
            actionList.push_back(action);
        } else {
            fillMenu();
        }
    }

    void RecentMenu::fillMenu()
    {
        foreach (const QString &path, resolver->getProjectsMap().keys()) {
            auto action = addAction(Helper::replaceHomePathInFullPath(path));

            actionList.push_back(action);

            if (!Saving::SaveManager::fileExists(path)) {
                action->setDisabled(true);
                continue;
            }

            connect(action, &QAction::triggered, this, [this, path] {
                emit openRecentProject(path);
            });
        }
    }
};
