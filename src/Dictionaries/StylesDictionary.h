//
// Created by artem on 31.01.2021.
//

#ifndef DBNODES_STYLESDICTIONARY_H
#define DBNODES_STYLESDICTIONARY_H

#include "QDir"
#include "QSettings"
#include "QDebug"

#include "StructDictionary.h"
#include "config.h"

namespace DbNodes::Dictionaries {

    struct StylesDictionary: public Abstract::Dictionary<QString, StylesDictionary>
    {
        public:
            static DictMap getDictionary(const QString &folder) {
                QDir directory(folder);
                auto stylesDirs = directory.entryList(QDir::AllDirs|QDir::NoDotAndDotDot);

                DictMap styles;

                foreach (const QString &styleDir, stylesDirs) {
                    auto configFile = folder + "/" + styleDir + "/config.ini";

                    if (!QFile::exists(configFile)) continue;

                    QSettings styleSettings(configFile, QSettings::IniFormat);

                    styles.insert(styleDir, styleSettings.value("info/title").toString());
                }

                return styles;
            };

            static DictMap getFoldersDictionary(const QString &folder) {
                return getDictionary(folder);
            }
    };

}

#endif //DBNODES_STYLESDICTIONARY_H
