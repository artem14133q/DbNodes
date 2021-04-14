//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_DBTABLETYPESFKDICTIONARY_H
#define DBNODES_DBTABLETYPESFKDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct DbTableTypesFkDictionary: public Abstract::Dictionary<int, DbTableTypesFkDictionary>
    {
        enum Type {
            Integer,
            Tinyint,
            Bigint
        };

        public:
            static DICT_MAP(int) initDictionary() {
                return {
                    { Integer  ,   "integer"   },
                    { Tinyint  ,   "tinyint"   },
                    { Bigint   ,   "bigint"    },
                };
            };
    };

}


#endif //DBNODES_DBTABLETYPESFKDICTIONARY_H
