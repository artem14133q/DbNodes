//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_STRUCTDICTIONARY_H
#define DBNODES_STRUCTDICTIONARY_H

#include "QVariant"
#include "QObject"

namespace DbNodes::Abstract {

    template<class T1, class T2>
    struct Dictionary: public QObject
    {
        typedef QHash<T1, QVariant> DictMap;

        static DictMap getDictionary()
        {
            return T2::getDictionary();
        }

        public:

            static QVariant getValue(const T1 &key)
            {
                return getDictionary().value(key);
            }

            static QVariantList getAllValues()
            {
                return QVariantList(getDictionary().values());
            }

            static QList<T1> getAllKeys()
            {
                return getDictionary().keys();
            }

            static bool has(const T1 &key)
            {
                return getDictionary().contains(key);
            }
    };
}

#endif //DBNODES_STRUCTDICTIONARY_H