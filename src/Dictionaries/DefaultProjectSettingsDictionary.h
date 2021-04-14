//
// Created by artem on 07.04.2021.
//

#ifndef DBNODES_DEFAULTPROJECTSETTINGSDICTIONARY_H
#define DBNODES_DEFAULTPROJECTSETTINGSDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct DefaultProjectSettingsDictionary: public Abstract::Dictionary<QString, DefaultProjectSettingsDictionary>
    {
        public:
            enum Type {
                name,
                width,
                height
            };

            Q_ENUM(Type);

            static DICT_MAP(QString) initDictionary() {
                return {
                    { getKey(name)  , "Undefined" },
                    { getKey(width)  , 4000        },
                    { getKey(height) , 4000        },
                };
            };
    };

}

#endif //DBNODES_DEFAULTPROJECTSETTINGSDICTIONARY_H
