//
// Created by artem on 10.03.2021.
//

#ifndef DBNODES_SAVEFILETYPESDICTIONARY_H
#define DBNODES_SAVEFILETYPESDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct SaveFileTypesDictionary: public Abstract::Dictionary<int, SaveFileTypesDictionary>
    {
        public:
            enum Type {
                NewFile,
                RewriteFile
            };

            static DictMap getDictionary() {
                return {
                    { NewFile       , "Create new save file"    },
                    { RewriteFile   , "Overwrite existing file" },
                };
            };
    };

}


#endif //DBNODES_SAVEFILETYPESDICTIONARY_H
