//
// Created by artem on 28.02.2021.
//

#ifndef DBNODES_RELATIONLINKPOSITIONSDICTIONARY_H
#define DBNODES_RELATIONLINKPOSITIONSDICTIONARY_H

#include "StructDictionary.h"

#define RELATION_LINK_POSITION_RIGHT 1
#define RELATION_LINK_POSITION_LEFT  2

namespace DbNodes::Dictionaries {

    class RelationLinkPositionsDictionary: public Abstract::Dictionary<int, RelationLinkPositionsDictionary>
    {
        public:
            static DICT_MAP(int) initDictionary() {
                return {
                        { RELATION_LINK_POSITION_RIGHT  ,   "right" },
                        { RELATION_LINK_POSITION_LEFT   ,   "left"  },
                };
            };
    };

}

#endif //DBNODES_RELATIONLINKPOSITIONSDICTIONARY_H
