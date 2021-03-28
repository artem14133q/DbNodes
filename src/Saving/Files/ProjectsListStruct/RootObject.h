//
// Created by artem on 23.03.2021.
//

#ifndef DBNODES_ROOTOBJECT_H
#define DBNODES_ROOTOBJECT_H

#include "JsonObject.h"
#include "ProjectDataObject.h"

namespace DbNodes::Saving::ProjectsListFileStruct {

    class RootObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(RootObject)

            STRING_PROPERTY("lastOpenedProject", getLastOpenedProject, setLastOpenedProject)
            LIST_PROPERTY(ProjectDataObject, "projects", getProjects, setProjects)
    };

}


#endif //DBNODES_ROOTOBJECT_H
