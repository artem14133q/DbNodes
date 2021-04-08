//
// Created by artem on 09.03.2021.
//

#ifndef DBNODES_RELATIONOBJECT_H
#define DBNODES_RELATIONOBJECT_H

#include "JsonObject.h"

namespace DbNodes::Saving::DbnFileStruct {

    class RelationObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(RelationObject)

            STRING_PROPERTY("id", getId, setId)
            STRING_PROPERTY("fkColumnId", getFkColumnId, setFkColumnId)
            STRING_PROPERTY("pkColumnId", getPkColumnId, setPkColumnId)
            INT_PROPERTY("type", getType, setType)
            INT_PROPERTY("position", getPosition, setPosition)
    };
}

#endif //DBNODES_RELATIONOBJECT_H
