//
// Created by artem on 28.02.2021.
//

#ifndef DBNODES_RELATIONPOSITIONSDICTIONARY_H
#define DBNODES_RELATIONPOSITIONSDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    class RelationPositionsDictionary: public Abstract::Dictionary<int, RelationPositionsDictionary>
    {
        public:

            enum Type {
                Undefined,
                Right,
                Left,
            };

            static DICT_MAP(int) initDictionary() {
                return {
                    { Right  ,   "right"     },
                    { Left   ,   "left"      },
                    { Undefined, "Undefined" }
                };
            };
    };

}

#endif //DBNODES_RELATIONPOSITIONSDICTIONARY_H
