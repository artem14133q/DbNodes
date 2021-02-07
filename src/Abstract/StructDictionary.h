//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_STRUCTDICTIONARY_H
#define DBNODES_STRUCTDICTIONARY_H

#include "QVariant"

namespace DbNodes::Abstract {

    template<typename T1, class T2>
    struct Dictionary
    {
        #define DICT_MAP(key_type) QHash<key_type, QVariant>

        static DICT_MAP(T1) getDictionary()
        {
            return T2::initDictionary();
        }

        static QVariant getValue(const T1 &key)
        {
            return getDictionary().value(key);
        }

        static QVariantList getAllValues()
        {
            return QVariantList(getDictionary().values());
        }
    };
}

#endif //DBNODES_STRUCTDICTIONARY_H