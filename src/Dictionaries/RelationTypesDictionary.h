//
// Created by artem on 28.02.2021.
//

#ifndef DBNODES_RELATIONTYPESDICTIONARY_H
#define DBNODES_RELATIONTYPESDICTIONARY_H

#include "StructDictionary.h"

#define RELATION_TYPE_PATH 1
#define RELATION_TYPE_LINK 2

namespace DbNodes::Dictionaries {

    class RelationTypesDictionary: public Abstract::Dictionary<int, RelationTypesDictionary>
    {
        public:
            static DICT_MAP(int) initDictionary() {
                return {
                    { RELATION_TYPE_PATH  ,   "Relation path" },
                    { RELATION_TYPE_LINK  ,   "Relation link" },
                };
            };
    };

}

#endif //DBNODES_RELATIONTYPESDICTIONARY_H
