//
// Created by artem on 12.04.2021.
//

#ifndef DBNODES_RELATIONPATHPOINTOBJECT_H
#define DBNODES_RELATIONPATHPOINTOBJECT_H

#include "JsonObject.h"

namespace DbNodes::Saving::DbnFileStruct {

    class RelationPathPointObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(RelationPathPointObject)

            INT_PROPERTY("x", getX, setX)
            INT_PROPERTY("y", getY, setY)
    };

}

#endif //DBNODES_RELATIONPATHPOINTOBJECT_H
