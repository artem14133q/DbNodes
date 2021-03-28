//
// Created by artem on 08.03.2021.
//

#ifndef DBNODES_PROJECTPARAMETERSOBJECT_H
#define DBNODES_PROJECTPARAMETERSOBJECT_H

#include "JsonObject.h"

namespace DbNodes::Saving::DbnFileStruct {

    class ProjectParametersObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(ProjectParametersObject)

            STRING_PROPERTY("name", getName, setName)
            INT_PROPERTY("workAreaWidth", getWorkAreaWidth, setWorkAreaWidth)
            INT_PROPERTY("workAreaHeight", getWorkAreaHeight, setWorkAreaHeight)
    };

}


#endif //DBNODES_PROJECTPARAMETERSOBJECT_H
