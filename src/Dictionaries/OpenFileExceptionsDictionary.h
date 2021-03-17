//
// Created by artem on 10.03.2021.
//

#ifndef DBNODES_OPENFILEEXCEPTIONSDICTIONARY_H
#define DBNODES_OPENFILEEXCEPTIONSDICTIONARY_H

#include "StructDictionary.h"

#define FILE_NOT_FOUND_EXCEPTION 0
#define INVALID_FILE_TYPE_EXCEPTION 1
#define CAN_NOT_OPEN_FILE_EXCEPTION 2
#define FILE_PATH_IS_EMPTY_EXCEPTION 3
#define FILE_IS_EMPTY_EXCEPTION 4

namespace DbNodes::Dictionaries {

    struct OpenFileExceptionsDictionary: public Abstract::Dictionary<int, OpenFileExceptionsDictionary>
    {
        public:
            static DICT_MAP(int) initDictionary() {
                return {
                    { FILE_NOT_FOUND_EXCEPTION, "Cannot find project file: '%1'"},
                    { INVALID_FILE_TYPE_EXCEPTION,"File type not valid"},
                    { CAN_NOT_OPEN_FILE_EXCEPTION,"Cannot open this file: '%1'"},
                    { FILE_PATH_IS_EMPTY_EXCEPTION,"File path is empty"},
                    { FILE_IS_EMPTY_EXCEPTION,"This file is empty"},
                };
            };
    };

}

#endif //DBNODES_OPENFILEEXCEPTIONSDICTIONARY_H
