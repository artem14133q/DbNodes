//
// Created by artem on 30.01.2021.
//

#include "MainSettings.h"
#include "config.h"

namespace DbNodes::Settings {

    MainSettings* MainSettings::instance = nullptr;

    MainSettings::MainSettings(): QSettings(CONFIG_FILE_PATH, QSettings::IniFormat) {}

    MainSettings *MainSettings::getInstance()
    {
        if (instance == nullptr) {
            instance = new MainSettings();
        }

        return instance;
    }

    void MainSettings::subscribe(const QString &settingName, const CONNECTOR_CALLBACK &callback)
    {
        callbacks.insert(settingName, callback);
    }

    bool MainSettings::has(const QString &key)
    {
        return callbacks.keys().contains(key);
    }

    void MainSettings::resolveCallback(const QString &key, const QVariant &value)
    {
        if (!has(key)) return;

        auto callback = callbacks.value(key);

        callback(value);
    }

    void MainSettings::unBind(const QString &key)
    {
        if (!has(key)) return;

        callbacks.remove(key);
    }

    QVariant MainSettings::get(const QString &key)
    {
        return MainSettings::getInstance()->value(key);
    }

    void MainSettings::set(const QString &key, const QVariant &value)
    {
        auto *settings = MainSettings::getInstance();

        settings->setValue(key, value);
        settings->resolveCallback(key, value);
    }
}

