//
// Created by artem on 31.01.2021.
//

#ifndef DBNODES_SETTINGS_H
#define DBNODES_SETTINGS_H

#include "QVBoxLayout"
#include "QPushButton"
#include "QLabel"

#include "AbstractModal.h"

#define VARIANTS_MAP QHash<QString, QVariant>

namespace DbNodes::Modals {

    class Settings : public Abstract::AbstractModal
    {
        public:
            explicit Settings(QWidget *parent = nullptr);
            void exit() override;

        private:
            bool canSave = false;

            QHash<QString, QVariant> settingsMap;
            QHash<QString, QVariant> newSettingsMap;

            QPushButton *pbApply;
            QPushButton *pbOk;

            QWidget *centralWidget{};
            QVBoxLayout *centralLayout{};

            QLabel *getTitle(const QString &name);

            void confirm() override;

            void initUI();

            void createTextSetting(const QString &name, const QString &settingKey);

            void createComboBoxSetting(const QString &name, const QString &settingKey, const VARIANTS_MAP &values);

            void createCheckboxSetting(const QString &name, const QString &settingKey);

            void createSettingGroup(const QString &name);

        protected:
            void accept(const bool &close = false);
            void changeSettingMap(const QString &name, const QVariant &value);
            void enableButtons(const bool &enable);
    };

}


#endif //DBNODES_SETTINGS_H
