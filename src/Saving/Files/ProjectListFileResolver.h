//
// Created by artem on 23.03.2021.
//

#ifndef DBNODES_PROJECTLISTFILERESOLVER_H
#define DBNODES_PROJECTLISTFILERESOLVER_H

#include "ProjectsListStruct/RootObject.h"
#include "FileResolver.h"
#include "SaveManager.h"

namespace DbNodes::Saving {

    class ProjectListFileResolver: public FileResolver<ProjectsListFileStruct::RootObject>
    {
        private:
            QHash<QString, QString> projects;

            QString filePath;

        public:
            explicit ProjectListFileResolver(SaveManager *saveManager, QString filePath);

            bool createFileIfNotExists(const QString &path) override;

            void appendNewProject(const QString &name, const QString &path);

            void updateProjectName(const QString &path, const QString &name);

            QHash<QString, QString> getProjectsMap();

            void removeProject(const QString &path);

            QString getLastOpenedPath();
            void setLastOpenedPath(const QString &path);

            QByteArray toJson() override;
            void fromJson() override;
    };

}

#endif //DBNODES_PROJECTLISTFILERESOLVER_H
