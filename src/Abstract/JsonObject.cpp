//
// Created by artem on 09.03.2021.
//

#include "QDebug"
#include "JsonObject.h"

namespace DbNodes::Abstract {

    JsonObject::JsonObject(const QByteArray &data)
    {
        if (data == nullptr) {
            json = QJsonObject();
        }

        json = QJsonDocument::fromJson(data).object();
    }

    QByteArray JsonObject::toJson() const
    {
        return QJsonDocument(json).toJson(QJsonDocument::Indented);
    }

    QJsonObject JsonObject::getBaseObject() const
    {
        return json;
    }

    void JsonObject::setBaseObject(const QJsonObject &object)
    {
        json = object;
    }
}