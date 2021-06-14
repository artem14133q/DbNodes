//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_DBTABLETYPESFKDICTIONARY_H
#define DBNODES_DBTABLETYPESFKDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct DbTableTypesFkDictionary: public Abstract::Dictionary<int, DbTableTypesFkDictionary>
    {
        public:
            enum Type {
                Integer,
                Tinyint,
                Bigint
            };

            static DictMap getDictionary() {
                return {
                    { Integer  ,   "integer"   },
                    { Tinyint  ,   "tinyint"   },
                    { Bigint   ,   "bigint"    },
                };
            };
    };

}


#endif //DBNODES_DBTABLETYPESFKDICTIONARY_H
