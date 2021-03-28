//
// Created by artem on 09.03.2021.
//

#ifndef DBNODES_TABLEOBJECT_H
#define DBNODES_TABLEOBJECT_H

#include "JsonObject.h"
#include "ColumnObject.h"

namespace DbNodes::Saving::DbnFileStruct {

    class TableObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(TableObject)

            STRING_PROPERTY("id", getId, setId)
            STRING_PROPERTY("name", getName, setName)
            INT_PROPERTY("x", getX, setX)
            INT_PROPERTY("y", getY, setY)
            LIST_PROPERTY(ColumnObject, "columns", getColumns, setColumns)
    };


}


#endif //DBNODES_TABLEOBJECT_H
