//
// Created by artem on 29.04.2021.
//

#include "MinimapSettings.h"
#include "MinimapPositionsDictionary.h"
#include "config.h"

namespace DbNodes::Modals {

    MinimapSettings::MinimapSettings(QWidget *parent) : Abstract::AbstractSettingModal(parent)
    {
        setWindowTitle("Minimap settings");
        setFixedWidth(500);

        setAcceptOnEnter(true);
        bindApplySettings(true);

        prepareWidget();

        show();
    }

    void MinimapSettings::enableConfirm(const bool &enable)
    {
        pbOk->setEnabled(enable);
        pbApply->setEnabled(enable);
        AbstractSettingModal::enableConfirm(enable);
    }

    void MinimapSettings::initSettingsUi()
    {
        createComboBoxSetting(
            "Position",
            "minimap.position",
            Dictionaries::MinimapPositionsDictionary::getPreparedDictionary()
        );

        auto slider = createSliderSetting(
            "Size",
            "minimap.size",
            {MINIMAP_MIN_CONTAINER_SIZE, MINIMAP_MAX_CONTAINER_SIZE}
        );

        slider->setSingleStep(1);
        slider->setPageStep(10);
        slider->setOrientation(Qt::Horizontal);

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

    void MinimapSettings::accept(const bool &close)
    {
        foreach (const QString &key, newSettingsMap.keys()) {
            Helper::setSettingValue(key, newSettingsMap.value(key));
        }

        AbstractSettingModal::accept(close);
    }
}