//
// Created by artem on 31.12.2020.
//

#ifndef DBNODES_SETTINGSMANAGER_H
#define DBNODES_SETTINGSMANAGER_H

#include "Parser.h"
#include "Writer.h"

namespace DbNodes::Settings {

    class SettingsManager
    {
        private:
            Writer *settingsWriter;
            Parser *settingsParser;

            const QString &settingFilePath;
            QHash<QString, QString> settings;

        protected:
            explicit SettingsManager(const QString &path);
            static SettingsManager *instance;

        public:
            SettingsManager(SettingsManager &other) = delete;
            SettingsManager(SettingsManager &&other) = delete;
            void operator=(const SettingsManager &) = delete;

            QHash<QString, QString> &getSettingsList();
            void updateFile();

            static SettingsManager* getInstance();

            static QHash<QString, QString> &getAllSettings();
            static QString getSetting(const QString &key);
            static bool updateSetting(const QString &key, const QString &value);
    };

}


#endif //DBNODES_SETTINGSMANAGER_H
