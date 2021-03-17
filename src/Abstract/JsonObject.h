//
// Created by artem on 09.03.2021.
//

#ifndef DBNODES_JSONOBJECT_H
#define DBNODES_JSONOBJECT_H

#include "QJsonObject"
#include "QJsonValue"
#include "QJsonArray"
#include "QJsonDocument"
#include "QVariant"
#include "QByteArray"
#include "QVector"

#define PROPERTY(type, field, getName, setName, toMethod) \
    [[nodiscard]] type getName() const { return json.value(field).toMethod(); } \
    void setName(const type &value) { json.insert(field, value); }

#define OBJECT_PROPERTY(type, field, getName, setName) \
    [[nodiscard]] type getName() const { type object; \
                                         object.setBaseObject(json.value(field).toObject()); \
                                         return object; } \
    void setName(const type &value) { json.insert(field, value.getBaseObject()); }

#define VECTOR_PROPERTY(type, field, getName, setName) \
    [[nodiscard]] QVector<type> getName() const { return Abstract::JsonObject::convertToVector<type>(json.value(field).toArray()); } \
    void setName(const QVector<type> &value) { json.insert(field, Abstract::JsonObject::convertToQJsonArray<type>(value.toList())); }

#define LIST_PROPERTY(type, field, getName, setName) \
    [[nodiscard]] QList<type> getName() const { return type::convertToVector<type>(json.value(field).toArray()).toList(); } \
    void setName(const QList<type> &value) { json.insert(field, type::convertToQJsonArray<type>(value)); }

#define JSON_OBJECT(type) \
    explicit type(const QByteArray &data = nullptr): Abstract::JsonObject(data) {}\
    template<typename type>\
    static QVector<type> convertToVector(const QJsonArray &data)\
    {QVector<type> vector;\
        foreach (const QVariant &variant, data) {\
            type object;\
            object.setBaseObject(variant.toJsonObject());\
            vector.push_back(object);\
        }\
        return vector;\
    }\
    template<typename type>\
    static QJsonArray convertToQJsonArray(const QList<type> &data)\
    {QJsonArray array;\
        foreach (const type &item, data) {\
            array.push_back(item.getBaseObject());\
        }\
        return array;\
    }\

#define INT_PROPERTY(field, getName, setName) PROPERTY(int, field, getName, setName, toInt)
#define STRING_PROPERTY(field, getName, setName) PROPERTY(QString, field, getName, setName, toString)
#define VARIANT_PROPERTY(field, getName, setName) PROPERTY(QVariant, field, getName, setName, toVariant)
#define BOOL_PROPERTY(field, getName, setName) PROPERTY(bool, field, getName, setName, toBool)
#define DOUBLE_PROPERTY(field, getName, setName) PROPERTY(double, field, getName, setName, toDouble)
#define JSON_OBJECT_PROPERTY(field, getName, setName) PROPERTY(QJsonObject, field, getName, setName, toObject)
#define JSON_ARRAY_PROPERTY(field, getName, setName) PROPERTY(QArrayObject, field, getName, setName, toArray)

namespace DbNodes::Abstract {

    class JsonObject
    {
        protected:
            QJsonObject json;

        public:
            explicit JsonObject(const QByteArray &data = nullptr);

            [[nodiscard]] QByteArray toJson() const;

            [[nodiscard]] QJsonObject getBaseObject() const;

            void setBaseObject(const QJsonObject &object);
    };

}

#endif //DBNODES_JSONOBJECT_H