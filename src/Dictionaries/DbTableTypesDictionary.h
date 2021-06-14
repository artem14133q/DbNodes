//
// Created by artem on 15.01.2021.
//

#ifndef DBNODES_DBTABLETYPESDICTIONARY_H
#define DBNODES_DBTABLETYPESDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct DbTableTypesDictionary: public Abstract::Dictionary<int, DbTableTypesDictionary>
    {
        public:
            enum Type {
                // Numeric types
                Float,
                Double,

                // Symbolic types
                Char,
                Text,
                Varchar,
                Longtext,

                // Time types
                Time,
                Date,
                Datetime,
                Timestamp,
                Year,

                // Other
                Binary,
                Bool,
                Json
            };

            static DictMap getDictionary() {
                return {
                    { Float     ,   "float"     },
                    { Double    ,   "double"    },

                    { Char      ,   "char"      },
                    { Varchar   ,   "varchar"   },
                    { Text      ,   "text"      },
                    { Longtext  ,   "longtext"  },

                    { Time      ,   "time"      },
                    { Date      ,   "date"      },
                    { Datetime  ,   "datetime"  },
                    { Timestamp ,   "timestamp" },
                    { Year      ,   "year"      },

                    { Binary    ,   "binary"    },
                    { Bool      ,   "bool"      },
                    { Json      ,   "json"      },
                };
            };
    };

}

#endif //DBNODES_DBTABLETYPESDICTIONARY_H
