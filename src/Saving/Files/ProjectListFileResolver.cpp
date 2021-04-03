//
// Created by artem on 23.03.2021.
//

#include "ProjectListFileResolver.h"
#include "QDebug"

namespace DbNodes::Saving {

    ProjectListFileResolver::ProjectListFileResolver(SaveManager *saveManager, QString filePath)
        : FileResolver(saveManager), filePath(std::move(filePath))
    {
        createFileIfNotExists(this->filePath);
        load(this->filePath);
    }

    void ProjectListFileResolver::appendNewProject(const QString &name, const QString &path)
    {
        if (!projects.contains(path)) {
            projects.insert(path, name);
            save(filePath);
        }
    }

    QHash<QString, QString> ProjectListFileResolver::getProjectsMap()
    {
        return projects;
    }

    void ProjectListFileResolver::removeProject(const QString &path)
    {
        projects.remove(path);

        save(filePath);
    }

    QByteArray ProjectListFileResolver::toJson()
    {
        QList<ProjectsListFileStruct::ProjectDataObject> projectObjectsList;

        foreach (const QString &key, projects.keys()) {
            ProjectsListFileStruct::ProjectDataObject projectObject;

            projectObject.setPath(key);
            projectObject.setTitile(projects.value(key));

            projectObjectsList.append(projectObject);
        }

        object->setProjects(projectObjectsList);

        return object->toJson();
    }

    void ProjectListFileResolver::fromJson()
    {
        foreach (const ProjectsListFileStruct::ProjectDataObject &projectObject, object->getProjects()) {
            projects.insert(projectObject.getPath(), projectObject.getTitle());
        }
    }

    QString ProjectListFileResolver::getLastOpenedPath()
    {
        return object->getLastOpenedProject();
    }

    void ProjectListFileResolver::setLastOpenedPath(const QString &path)
    {
        object->setLastOpenedProject(path);

        save(filePath);
    }

    bool ProjectListFileResolver::createFileIfNotExists(const QString &path)
    {
        auto result = FileResolver::createFileIfNotExists(path);

        if (result) {
            object->setLastOpenedProject("");
            object->setProjects({});

            save(path);
        }

        return result;
    }

    void ProjectListFileResolver::updateProjectName(const QString &path, const QString &name)
    {
        if (projects.contains(path)) {
            projects.insert(path, name);

            save(filePath);
        }
    }
}