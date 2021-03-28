//
// Created by artem on 08.03.2021.
//

#ifndef DBNODES_COLUMNOBJECT_H
#define DBNODES_COLUMNOBJECT_H

#include "JsonObject.h"

namespace DbNodes::Saving::DbnFileStruct {

    class ColumnObject: public Abstract::JsonObject
    {
        public:
            JSON_OBJECT(ColumnObject)

            STRING_PROPERTY("id", getId, setId);
            STRING_PROPERTY("name", getName, setName)
            STRING_PROPERTY("dbType", getDbType, setDbType)
            INT_PROPERTY("columnType", getColumnType, setColumnType)
            BOOL_PROPERTY("nullable", getNullable, setNullable)
    };

}

#endif //DBNODES_COLUMNOBJECT_H
