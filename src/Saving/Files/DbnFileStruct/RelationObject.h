//
// Created by artem on 09.03.2021.
//

#ifndef DBNODES_RELATIONOBJECT_H
#define DBNODES_RELATIONOBJECT_H

#include "JsonObject.h"

namespace DbNodes::Saving {

    class RelationObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(RelationObject)

            STRING_PROPERTY("id", getId, setId)
            STRING_PROPERTY("fkNodeRawId", getFkNodeRawId, setFkNodeRawId)
            STRING_PROPERTY("pkNodeRawId", getPkNodeRawId, setPkNodeRawId)
            INT_PROPERTY("type", getType, setType)
            INT_PROPERTY("position", getPosition, setPosition)
    };
}

#endif //DBNODES_RELATIONOBJECT_H
