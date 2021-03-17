//
// Created by artem on 31.01.2021.
//

#include "QVBoxLayout"
#include "QLabel"
#include "QCheckBox"
#include "QLineEdit"
#include "QComboBox"
#include "QScrollArea"
#include "QDir"

#include "Settings.h"
#include "StylesDictionary.h"

#include "../helper.h"
#include "config.h"

namespace DbNodes::Modals {

    Settings::Settings(QWidget *parent) : AbstractModal(parent)
    {
        setFixedSize(600, 400);
        setWindowTitle("Settings");

        // Disabled MainWindow then relation maker open
        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(true);

        // Enabled this window
        setEnabled(true);

        setStyleSheet(Helper::getStyleFromFile("subWindow"));

        initUI();

        newSettingsMap = settingsMap;

        show();
    }

    void Settings::initUI()
    {
        auto pbStyle = Helper::getStyleFromFile("button");

        pbOk = new QPushButton("Ok", this);
        pbOk->setFixedSize(70, 30);
        pbOk->setStyleSheet(pbStyle);
        pbOk->setDisabled(true);

        pbApply = new QPushButton("Apply", this);
        pbApply->setFixedSize(70, 30);
        pbApply->setStyleSheet(pbStyle);
        pbApply->setDisabled(true);

        connect(pbApply, &QPushButton::clicked, this, [this] () {
            accept();
        });

        connect(pbOk, &QPushButton::clicked, this, [this] () {
            accept(true);
        });

        auto *widget = new QWidget(this);
        auto *layout = new QVBoxLayout(widget);

        auto *scrollArea = new QScrollArea(this);

        centralWidget = new QWidget(scrollArea);
        centralWidget->setFixedWidth(this->width() - 40);
        centralLayout = new QVBoxLayout(centralWidget);

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

        centralLayout->setSpacing(15);

        createSettingGroup("Rendering:");

        createCheckboxSetting(
            "Antialiasing",
            "rendering.antialiasing"
        );

        centralLayout->setSpacing(15);
        centralLayout->addStretch();

        centralWidget->setLayout(centralLayout);

        scrollArea->setWidget(centralWidget);
        layout->addWidget(scrollArea);
        layout->addStretch();

        auto *pbLayout = new QHBoxLayout(layout->widget());
        pbLayout->addStretch();
        pbLayout->setSpacing(10);

        auto *pbClose = new QPushButton("Close", this);
        pbClose->setFixedSize(70, 30);
        pbClose->setStyleSheet(pbStyle);
        connect(pbClose, &QPushButton::clicked, this, &Settings::exit);
        pbLayout->addWidget(pbClose);

        pbLayout->addWidget(pbOk);
        pbLayout->addWidget(pbApply);

        layout->addLayout(pbLayout);

        widget->setLayout(layout);
        setCentralWidget(widget);
    }

    void Settings::createSettingGroup(const QString &name)
    {
        auto *hl = new QHBoxLayout();

        auto *title = getTitle(name);
        title->setStyleSheet(title->styleSheet() + "QLabel{font-size: 18px;}");

        auto *line = new QLabel("", centralWidget);
        line->setFixedHeight(1);
        line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        line->setStyleSheet("background-color: #666666;");

        hl->addWidget(title);
        hl->addWidget(line);

        centralLayout->addLayout(hl);
    }

    void Settings::createCheckboxSetting(const QString &name, const QString &settingKey)
    {
        auto *hl = new QHBoxLayout();

        auto *checkBox = new QCheckBox(centralWidget);
        checkBox->setStyleSheet(Helper::getStyleFromFile("settingsCheckBox"));

        auto state = Helper::getSettingValue(settingKey).toBool();

        if (state) checkBox->setCheckState(Qt::Checked);

        connect(checkBox, &QCheckBox::stateChanged, this, [this, settingKey] (const int &state) {
            changeSettingMap(settingKey, state ? true : false);
        });

        settingsMap.insert(settingKey, state);

        hl->addSpacing(30);
        hl->addWidget(checkBox);
        hl->addWidget(getTitle(name));
        hl->addStretch();

        centralLayout->addLayout(hl);
    }

    QLabel *Settings::getTitle(const QString &name)
    {
        auto *label = new QLabel(name, centralWidget);
        label->setStyleSheet(Helper::getStyleFromFile("title"));

        return label;
    }

    void Settings::createTextSetting(const QString &name, const QString &settingKey)
    {
        auto *hl = new QHBoxLayout();

        hl->addSpacing(30);
        hl->addWidget(getTitle(name));

        auto value = Helper::getSettingValue(settingKey);

        auto *lineEdit = new QLineEdit(centralWidget);
        lineEdit->setStyleSheet(Helper::getStyleFromFile("lineEdit"));
        lineEdit->setText(value.toString());

        connect(lineEdit, &QLineEdit::textEdited, this, [this, settingKey] (const QString &text) {
            changeSettingMap(settingKey, text);
        });

        settingsMap.insert(settingKey, value);

        hl->addWidget(lineEdit);

        centralLayout->addLayout(hl);
    }

    void Settings::createComboBoxSetting(const QString &name, const QString &settingKey, const VARIANTS_MAP &values)
    {
        auto *hl = new QHBoxLayout();

        hl->addSpacing(30);
        hl->addWidget(getTitle(name));

        auto *comboBox = new QComboBox(centralWidget);
        comboBox->setStyleSheet(Helper::getStyleFromFile("settingsComboBox"));
        comboBox->setFixedWidth(200);

        foreach (const QString &key, values.keys()) {
            comboBox->addItem(values.value(key).toString(), key);
        }

        auto value = Helper::getSettingValue(settingKey);

        comboBox->setCurrentIndex(comboBox->findData(value));

        connect(comboBox, &QComboBox::currentTextChanged, this, [this, settingKey, comboBox] (const QString &) {
            changeSettingMap(settingKey, comboBox->currentData());
        });

        settingsMap.insert(settingKey, value);

        hl->addStretch();
        hl->addWidget(comboBox);
        hl->addSpacing(100);

        centralLayout->addLayout(hl);
    }

    void Settings::exit()
    {
        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(false);
        AbstractModal::exit();
    }

    void Settings::changeSettingMap(const QString &name, const QVariant &value)
    {
        newSettingsMap.insert(name, value);

        foreach (const QString &key, settingsMap.keys()) {
            auto oldValue = settingsMap.value(key);

            if (oldValue != newSettingsMap.value(key)) {
                enableButtons(true);
                return;
            }

            enableButtons(false);
        }
    }

    void Settings::accept(const bool &close)
    {
        foreach (const QString &key, newSettingsMap.keys()) {
            Helper::setSettingValue(key, newSettingsMap.value(key));
        }

        if (close) confirm();

        enableButtons(false);
        settingsMap.clear();
        settingsMap = newSettingsMap;
    }

    void Settings::confirm()
    {
        if (!canSave) return;

        AbstractModal::confirm();
    }

    void Settings::enableButtons(const bool &enable)
    {
        pbOk->setEnabled(enable);
        pbApply->setEnabled(enable);
        canSave = enable;
    }
}
