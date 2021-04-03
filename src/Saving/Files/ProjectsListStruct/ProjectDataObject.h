//
// Created by artem on 23.03.2021.
//

#ifndef DBNODES_PROJECTDATAOBJECT_H
#define DBNODES_PROJECTDATAOBJECT_H

#include "JsonObject.h"

namespace DbNodes::Saving::ProjectsListFileStruct {

    class ProjectDataObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(ProjectDataObject)

            STRING_PROPERTY("title", getTitle, setTitile)
            STRING_PROPERTY("path", getPath, setPath)
    };

}

#endif //DBNODES_PROJECTDATAOBJECT_H
