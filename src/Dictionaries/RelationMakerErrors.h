//
// Created by artem on 18.01.2021.
//

#ifndef DBNODES_RELATIONMAKERERRORS_H
#define DBNODES_RELATIONMAKERERRORS_H

#include "StructDictionary.h"

#define         CANNOT_FIND_PK_NODE_ROWS        0
#define         CANNOT_FIND_NODES               1

namespace DbNodes::Dictionaries {

    struct RelationMakerErrors: public Abstract::Dictionary<int, RelationMakerErrors>
    {
        public:
            static DICT_MAP(int) initDictionary() {
                return {
                    { CANNOT_FIND_PK_NODE_ROWS  ,   "Cannot find primary keys in current table" },
                    { CANNOT_FIND_NODES         ,   "Cannot find tables"                        },
                };
            };
    };

}

#endif //DBNODES_RELATIONMAKERERRORS_H
