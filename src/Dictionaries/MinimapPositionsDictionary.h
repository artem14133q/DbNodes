//
// Created by artem on 04.05.2021.
//

#ifndef DBNODES_MINIMAPPOSITIONSDICTIONARY_H
#define DBNODES_MINIMAPPOSITIONSDICTIONARY_H

#include "StructDictionary.h"

namespace DbNodes::Dictionaries {

    struct MinimapPositionsDictionary: public Abstract::Dictionary<int, MinimapPositionsDictionary>
    {
        // type: 0 ---------- 0 ----------- 0 ----------- 0
        //       ^            ^             ^             ^
        //     Bottom        Top          Right          Left
        //
        // if (position & MinimapPositionsDictionary::Left) {
        //      do something ...
        // }

        public:
            enum Side {
                Left            = 1,    // 0001
                Right           = 2,    // 0010
                Top             = 4,    // 0100
                Bottom          = 8,    // 1000
            };

            enum Type {
                TopLeft         = 5,    // 0101
                TopRight        = 6,    // 0110
                BottomLeft      = 9,    // 1001
                BottomRight     = 10,   // 1010
            };

            /**
             * @return minimap positions dictionary
             */
            static DictMap getDictionary() {
                return {
                    { TopLeft       , "Top Left"        },
                    { TopRight      , "Top Right"       },
                    { BottomLeft    , "Bottom Left"     },
                    { BottomRight   , "Bottom Right"    },
                };
            };

            /**
             * @return dictionary with QString type key
             */
            static QHash<QString, QVariant> getPreparedDictionary()
            {
                QHash<QString, QVariant> preparedHash;
                auto dict = getDictionary();

                foreach (const int &key, dict.keys()) {
                    preparedHash.insert(QString::number(key), dict.value(key));
                }

                return preparedHash;
            }
    };
}

#endif //DBNODES_MINIMAPPOSITIONSDICTIONARY_H
