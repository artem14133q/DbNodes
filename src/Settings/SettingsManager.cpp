//
// Created by artem on 31.12.2020.
//

#include "SettingsManager.h"
#include "config.h"
#include "QDebug"

namespace DbNodes::Settings {

    SettingsManager * SettingsManager::instance = nullptr;

    SettingsManager::SettingsManager(const QString &path) : settingFilePath(path)
    {
        settingsParser = new Parser(path);
        settingsWriter = new Writer(settingsParser, path);

        settings = QHash<QString, QString>(settingsParser->parseItems()->getSettings());
    }

    SettingsManager *SettingsManager::getInstance()
    {
        if (SettingsManager::instance == nullptr) {
            SettingsManager::instance = new SettingsManager(SETTINGS_FILE_PATH);
        }

        return SettingsManager::instance;
    }

    QHash<QString, QString> &SettingsManager::getSettingsList()
    {
        return settings;
    }

    bool SettingsManager::updateSetting(const QString &key, const QString &value)
    {
        auto *manager = SettingsManager::getInstance();

        auto settings = manager->getSettingsList();

        if (!settings.contains(key)) return false;

        settings.insert(key, value);

        manager->updateFile();

        return true;
    }

    QHash<QString, QString> &SettingsManager::getAllSettings()
    {
        auto *manager = SettingsManager::getInstance();

        return const_cast<QHash<QString, QString> &>(manager->getSettingsList());
    }

    QString SettingsManager::getSetting(const QString &key)
    {
        return SettingsManager::getAllSettings().value(key.toUpper());
    }

    void SettingsManager::updateFile()
    {
        settingsWriter->update(settings);
        settingsWriter->writeFile();

        settingsParser->parseItems();
    }
}