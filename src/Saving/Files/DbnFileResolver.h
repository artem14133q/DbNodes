//
// Created by artem on 10.03.2021.
//

#ifndef DBNODES_DBNFILERESOLVER_H
#define DBNODES_DBNFILERESOLVER_H

#include "Files/DbnFileStruct/ProjectObject.h"
#include "FileResolver.h"
#include "SaveManager.h"
#include "Workarea.h"

namespace DbNodes::Saving {

    class DbnFileResolver: public FileResolver<DbnFileStruct::ProjectObject>
    {
        private:
            Widgets::WorkArea *workArea;

            void fillProjectParameters();
            void fillTables();
            void fillColumns(DbnFileStruct::TableObject &tableObject, const Nodes::TablePtr &table);
            void fillRelations();

            void fillRelationPathPoints(
                DbnFileStruct::RelationObject &relationObject,
                const Relations::RelationPtr &relation
            );

            void loadProjectParameters();
            void loadTables();
            void loadColumns(const DbnFileStruct::TableObject &tableObject, Nodes::TablePtr &table);
            void loadRelations();

            void loadRelationPathPoints(
                const DbnFileStruct::RelationObject &relationObject,
                const Relations::RelationPtr &relation
            );

        public:
            explicit DbnFileResolver(SaveManager *saveManager, Widgets::WorkArea *workArea);

            QByteArray toJson() override;
            void fromJson() override;

            QString getProjectName();
    };

}

#endif //DBNODES_DBNFILERESOLVER_H
