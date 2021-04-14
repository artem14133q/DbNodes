//
// Created by artem on 28.02.2021.
//

#ifndef DBNODES_RELATIONTYPESDICTIONARY_H
#define DBNODES_RELATIONTYPESDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    class RelationTypesDictionary: public Abstract::Dictionary<int, RelationTypesDictionary>
    {
        public:
            enum Type {
                Undefined,
                Path,
                Link
            };

            static DICT_MAP(int) initDictionary() {
                return {
                    { Path          ,   "Relation path" },
                    { Link          ,   "Relation link" },
                    { Undefined     ,   "Undefined"     },
                };
            };
    };

}

#endif //DBNODES_RELATIONTYPESDICTIONARY_H
