//
// Created by artem on 27.12.2020.
//

#ifndef DBNODES_PARSER_H
#define DBNODES_PARSER_H

#include "QString"
#include "QHash"

namespace DbNodes::Settings {

    class Parser
    {
        private:
            QString content;
            const QString &settingFilePath;

            QHash<QString, QString> settings;

        protected:
            bool isNotValid(const QString &line);
            void readSettingsFile();

        public:
            explicit Parser(const QString &path);

            Parser *parseItems();

            const QHash<QString, QString> &getSettings();
            QString &getContent();
    };

}

#endif //DBNODES_PARSER_H
