//
// Created by artem on 15.01.2021.
//

#ifndef DBNODES_DBTABLETYPESDICTIONARY_H
#define DBNODES_DBTABLETYPESDICTIONARY_H

#include "StructDictionary.h"

// ========== Numeric types =======
#define         FLOAT           0
#define         DOUBLE          1

// ========== Char types ==========
#define         CHAR            2
#define         TEXT            3
#define         VARCHAR         4
#define         LONGTEXT        5

// ========== Time types ==========
#define         TIME            6
#define         DATA            7
#define         DATETIME        8
#define         TIMESTAMP       9
#define         YEAR            10

// ========== Other ===============
#define         BINARY          11
#define         BOOL            12
#define         JSON            13

namespace DbNodes::Dictionaries {

    struct DbTableTypesDictionary: public DbNodes::Abstract::Dictionary<DbTableTypesDictionary>
    {
        public:
            static DICT_LIST initDictionary() {
                return {
                    { FLOAT     ,   "float"     },
                    { DOUBLE    ,   "double"    },

                    { CHAR      ,   "char"      },
                    { VARCHAR   ,   "varchar"   },
                    { TEXT      ,   "text"      },
                    { LONGTEXT  ,   "longtext"  },

                    { TIME      ,   "time"      },
                    { DATA      ,   "date"      },
                    { DATETIME  ,   "datetime"  },
                    { TIMESTAMP ,   "timestamp" },
                    { YEAR      ,   "year"      },

                    { BINARY    ,   "binary"    },
                    { BOOL      ,   "bool"      },
                    { JSON      ,   "json"      },
                };
            };
    };

}

#endif //DBNODES_DBTABLETYPESDICTIONARY_H
