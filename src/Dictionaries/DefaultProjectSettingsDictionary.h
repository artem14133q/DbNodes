//
// Created by artem on 07.04.2021.
//

#ifndef DBNODES_DEFAULTPROJECTSETTINGSDICTIONARY_H
#define DBNODES_DEFAULTPROJECTSETTINGSDICTIONARY_H

#include "StructDictionary.h"

#define NAME_KEY "name"
#define WIDTH_KEY "width"
#define HEIGHT_KEY "height"

namespace DbNodes::Dictionaries {

    struct DefaultProjectSettingsDictionary: public Abstract::Dictionary<QString, DefaultProjectSettingsDictionary>
    {
        public:
            static DICT_MAP(QString) initDictionary() {
                return {
                        { NAME_KEY   , "Undefined" },
                        { WIDTH_KEY  , 4000        },
                        { HEIGHT_KEY , 4000        },
                };
            };
    };

}

#endif //DBNODES_DEFAULTPROJECTSETTINGSDICTIONARY_H
