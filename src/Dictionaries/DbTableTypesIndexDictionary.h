//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_DBTABLETYPESINDEXDICTIONARY_H
#define DBNODES_DBTABLETYPESINDEXDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct DbTableTypesIndexDictionary: public Abstract::Dictionary<int, DbTableTypesIndexDictionary>
    {
        public:
            enum Type {
                Integer,
                Tinyint,
                Bigint
            };

            /**
             * @return autoincrement database types dictionary
             */
            static DictMap getDictionary() {
                return {
                    { Integer  ,   "integer"   },
                    { Tinyint  ,   "tinyint"   },
                    { Bigint   ,   "bigint"    },
                };
            };
    };

}


#endif //DBNODES_DBTABLETYPESINDEXDICTIONARY_H
