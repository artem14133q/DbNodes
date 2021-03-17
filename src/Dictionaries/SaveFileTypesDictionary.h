//
// Created by artem on 10.03.2021.
//

#ifndef DBNODES_SAVEFILETYPESDICTIONARY_H
#define DBNODES_SAVEFILETYPESDICTIONARY_H

#define SAVE_TYPE_NEW_FILE 0
#define SAVE_TYPE_REWRITE_FILE 1

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct SaveFileTypesDictionary: public Abstract::Dictionary<int, SaveFileTypesDictionary>
    {
        public:
            static DICT_MAP(int) initDictionary() {
                return {
                    { SAVE_TYPE_NEW_FILE, "Create new save file"},
                    { SAVE_TYPE_REWRITE_FILE, "Overwrite existing file"},
                };
            };
    };

}


#endif //DBNODES_SAVEFILETYPESDICTIONARY_H
