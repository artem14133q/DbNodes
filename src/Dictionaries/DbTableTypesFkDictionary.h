//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_DBTABLETYPESFKDICTIONARY_H
#define DBNODES_DBTABLETYPESFKDICTIONARY_H

#include "StructDictionary.h"

// =========== Numeric types =========
#define         TYPE_INTEGER        0
#define         TYPE_TINYINT        1
#define         TYPE_BIGINT         2

namespace DbNodes::Dictionaries {

    struct DbTableTypesFkDictionary: public DbNodes::Abstract::Dictionary<DbTableTypesFkDictionary>
    {
        public:
            static DICT_LIST initDictionary() {
                return {
                    { TYPE_INTEGER  ,   "integer"   },
                    { TYPE_TINYINT  ,   "tinyint"   },
                    { TYPE_BIGINT   ,   "bigint"    },
                };
            };
    };

}


#endif //DBNODES_DBTABLETYPESFKDICTIONARY_H
