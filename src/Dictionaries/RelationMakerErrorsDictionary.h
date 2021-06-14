//
// Created by artem on 18.01.2021.
//

#ifndef DBNODES_RELATIONMAKERERRORSDICTIONARY_H
#define DBNODES_RELATIONMAKERERRORSDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct RelationMakerErrorsDictionary: public Abstract::Dictionary<int, RelationMakerErrorsDictionary>
    {
        public:
            enum Type {
                CannotFindPkColumns,
                CannotFindTables
            };

            static DictMap getDictionary() {
                return {
                    { CannotFindPkColumns  ,   "Cannot find primary keys in current table" },
                    { CannotFindTables     ,   "Cannot find tables"                        },
                };
            };
    };

}

#endif //DBNODES_RELATIONMAKERERRORSDICTIONARY_H
