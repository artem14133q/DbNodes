//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_STRUCTDICTIONARY_H
#define DBNODES_STRUCTDICTIONARY_H

#include "QVariant"

#define DICT_LIST QHash<int, QVariant>

namespace DbNodes::Abstract {

    template<class T>
    struct Dictionary
    {
        static DICT_LIST getDictionary()
        {
            return T::initDictionary();
        }

        static QVariant getValue(const int &key)
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