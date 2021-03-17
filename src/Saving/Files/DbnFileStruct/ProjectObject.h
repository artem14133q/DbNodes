//
// Created by artem on 08.03.2021.
//

#ifndef DBNODES_PROJECTOBJECT_H
#define DBNODES_PROJECTOBJECT_H

#include "JsonObject.h"

#include "ProjectParametersObject.h"
#include "TableObject.h"
#include "RelationObject.h"

namespace DbNodes::Saving {

    class ProjectObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(ProjectObject)

            OBJECT_PROPERTY(ProjectParametersObject, "projectParameters", getProjectParameters, setProjectParameters)
            LIST_PROPERTY(TableObject, "tables", getTables, setTables)
            LIST_PROPERTY(RelationObject, "relations", getRelations, setRelations)
    };

}


#endif //DBNODES_PROJECTOBJECT_H
