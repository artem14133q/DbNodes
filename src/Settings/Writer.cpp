//
// Created by artem on 31.12.2020.
//

#include "Writer.h"
#include "QFile"
#include "QTextStream"
#include "QRegExp"

namespace DbNodes::Settings {

    Writer::Writer(Parser *parser, const QString &path): parser(parser), settingsFilePath(path) {}

    void Writer::writeFile()
    {
        QFile file(settingsFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);

        out << parser->getContent();
    }

    void Writer::update(const QHash<QString, QString> &settings)
    {
        auto oldSettings = parser->getSettings();
        auto content = parser->getContent();

        foreach (const QString &key, settings.keys()) {
            auto replaceString = createSettingString(key, oldSettings.value(key));
            auto newString = createSettingString(key, settings.value(key));

            content = content.replace(QRegExp("^" + replaceString), newString);
        }
    }

    QString Writer::createSettingString(const QString &key, const QString &value)
    {
        return QString(key + "=" + value);
    }

}
