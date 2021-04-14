//
// Created by artem on 11.01.2021.
//

#ifndef DBNODES_STRUCTDICTIONARY_H
#define DBNODES_STRUCTDICTIONARY_H

#include "QVariant"
#include "QMetaEnum"
#include "QObject"

#define DICT_MAP(key_type) QHash<key_type, QVariant>

namespace DbNodes::Abstract {

    template<typename T1, class T2>
    struct Dictionary: public QObject
    {
        static DICT_MAP(T1) getDictionary()
        {
            return T2::initDictionary();
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

            template<typename QEnum>
            static QString getKey(const QEnum value)
            {
                return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
            }
    };
}

#endif //DBNODES_STRUCTDICTIONARY_H