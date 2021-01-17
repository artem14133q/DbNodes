//
// Created by artem on 31.12.2020.
//

#ifndef DBNODES_WRITER_H
#define DBNODES_WRITER_H

#include "QString"
#include "Parser.h"
#include "QHash"

namespace DbNodes::Settings {

    class Writer
    {
        private:
            Parser* parser;
            const QString &settingsFilePath;

        protected:
            QString createSettingString(const QString &key, const QString &value);

        public:
            explicit Writer(Parser* parser, const QString &path);
            void update(const QHash<QString, QString> &settings);
            void writeFile();

    };

}


#endif //DBNODES_WRITER_H
