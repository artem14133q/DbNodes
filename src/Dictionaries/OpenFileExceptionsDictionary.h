//
// Created by artem on 10.03.2021.
//

#ifndef DBNODES_OPENFILEEXCEPTIONSDICTIONARY_H
#define DBNODES_OPENFILEEXCEPTIONSDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct OpenFileExceptionsDictionary: public Abstract::Dictionary<int, OpenFileExceptionsDictionary>
    {
        public:
            enum Type {
                FileNotFound,
                InvalidFileType,
                CanNotOpenFile,
                FilePathIsEmpty,
                FileIsEmpty
            };

            static DICT_MAP(int) initDictionary() {
                return {
                    { FileNotFound, "Cannot find project file: '%1'"},
                    { InvalidFileType,"File type not valid"},
                    { CanNotOpenFile,"Cannot open this file: '%1'"},
                    { FilePathIsEmpty,"File path is empty"},
                    { FileIsEmpty,"This file is empty"},
                };
            };
    };

}

#endif //DBNODES_OPENFILEEXCEPTIONSDICTIONARY_H
