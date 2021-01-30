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
}

