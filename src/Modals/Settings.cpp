//
// Created by artem on 31.01.2021.
//

#include "QLabel"

#include "Settings.h"
#include "StylesDictionary.h"

#include "../helper.h"
#include "config.h"

namespace DbNodes::Modals {

    Settings::Settings(QWidget *parent) : Abstract::AbstractSettingModal(parent)
    {
        setFixedSize(600, 400);
        setWindowTitle("Settings");

        bindApplySettings(true);
        setAcceptOnEnter(true);

        prepareWidget();

        show();
    }

    void Settings::initSettingsUi()
    {
        setSettingIndent(10);

        createSettingGroup("Style:");

        createComboBoxSetting(
            "Stylesheet",
            "style.stylesheet_folder",
            Dictionaries::StylesDictionary::getFoldersDictionary(STYLE_FILES_PATH)
        );

        createComboBoxSetting(
            "Icons",
            "style.icons_folder",
            Dictionaries::StylesDictionary::getFoldersDictionary(ICONS_FILES_PATH)
        );

        setSpacing(15);

        createSettingGroup("Rendering:");

        createCheckboxSetting(
            "Antialiasing",
            "rendering.antialiasing"
        );

        pbOk = createButton("Ok", this, [this] {
            accept(true);
        });

        pbApply = createButton("Apply", this, [this] {
            accept(false);
        });

        createButton("Close", this, [this] {
            exit();
        });

        pbOk->setDisabled(true);
        pbApply->setDisabled(true);
    }

    void Settings::enableConfirm(const bool &enable)
    {
        pbOk->setEnabled(enable);
        pbApply->setEnabled(enable);
        AbstractSettingModal::enableConfirm(enable);
    }

    void Settings::accept(const bool &close)
    {
        foreach (const QString &key, newSettingsMap.keys()) {
            Helper::setSettingValue(key, newSettingsMap.value(key));
        }

        AbstractSettingModal::accept(close);
    }
}
