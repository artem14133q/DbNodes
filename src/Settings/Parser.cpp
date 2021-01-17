//
// Created by artem on 27.12.2020.
//

#include "Parser.h"
#include "QFile"

namespace DbNodes::Settings {

    Parser::Parser(const QString &path): settingFilePath(path)
    {
        readSettingsFile();
    }

    Parser *Parser::parseItems()
    {
        auto lines = content.trimmed().split("\n");

        foreach (const QString &line, lines) {
            if (isNotValid(line)) {
                lines.removeAll(line);
            }
        }

        settings.reserve(lines.size());

        foreach (const QString &line, lines) {
            auto data = line.trimmed().split("=");

            if (data.size() <= 1) {
                settings.insert(data[0], "");
                continue;
            }

            settings.insert(data[0], data[1]);
        }

        return this;
    }

    bool Parser::isNotValid(const QString &line)
    {
        return line.isEmpty() || line.startsWith("#");
    }

    void Parser::readSettingsFile()
    {
        QFile file(settingFilePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            content = "";

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            content += line;
        }
    }

    QString &Parser::getContent()
    {
        return content;
    }

    const QHash<QString, QString> &Parser::getSettings()
    {
        return settings;
    }
}
