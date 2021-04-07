//
// Created by artem on 06.04.2021.
//

#ifndef DBNODES_ABSTRACTSETTINGMODAL_H
#define DBNODES_ABSTRACTSETTINGMODAL_H

#include "QLineEdit"
#include "QCheckBox"
#include "QComboBox"
#include "QScrollArea"
#include "QLabel"
#include "QVBoxLayout"
#include "QPushButton"
#include "QSpinBox"
#include "QPoint"
#include "functional"

#include "AbstractModal.h"

#define VARIANTS_MAP QHash<QString, QVariant>
#define EVENT_CALLBACK(type) std::function<void (type *)>

namespace DbNodes::Abstract
{

    class AbstractSettingModal: public AbstractModal
    {
        public:
            explicit AbstractSettingModal(QWidget *parent = nullptr);

            void exit() override;
            void confirm() override;

            QLineEdit *createTextSetting(
                const QString &name,
                const QString &settingKey,
                const EVENT_CALLBACK(QLineEdit) &callback = nullptr
            );

            QComboBox *createComboBoxSetting(
                const QString &name,
                const QString &settingKey,
                const VARIANTS_MAP &values,
                const EVENT_CALLBACK(QComboBox) &callback = nullptr
            );

            QCheckBox *createCheckboxSetting(
                const QString &name,
                const QString &settingKey,
                const EVENT_CALLBACK(QCheckBox) &callback = nullptr
            );

            QSpinBox *createSpinboxSetting(
                const QString &name,
                const QString &settingKey,
                const QPoint &range = QPoint(),
                const EVENT_CALLBACK(QSpinBox) &callback = nullptr
            );

            void createSettingGroup(const QString &name);

            void setSpacing(int size);
            void addStretch();

            void bindApplySettings(const bool &enable);

            void prepareWidget();

        private:
            QWidget *mainWindow;

            QWidget *scrollWidget{};
            QVBoxLayout *scrollLayout{};
            QHBoxLayout *buttonsLayout{};

            QString pbStyle;

            bool applySettings = false;

            bool canSave = false;
            void initUI();
            int settingIndent = 0;

        protected:
            QHash<QString, QVariant> settingsMap;
            QHash<QString, QVariant> newSettingsMap;

            QLabel *getTitle(const QString &name);
            QLabel *getLine();

            virtual void initSettingsUi();

            void accept(const bool &close) override;

            virtual void beforeInitUi();
            virtual void afterInitUi();

            virtual QVariant getDefaultSetting(const QString &name);

            void changeSettingMap(const QString &name, const QVariant &value);

            virtual void enableConfirm(const bool &enable);

            void setSettingIndent(const int &size);

            QPushButton *createButton(const QString &name, QObject *target, const std::function<void ()> &callback);
    };

}

#endif //DBNODES_ABSTRACTSETTINGMODAL_H
