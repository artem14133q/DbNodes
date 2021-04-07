//
// Created by artem on 06.04.2021.
//

#include "AbstractSettingModal.h"
#include "QDebug"

#include "../helper.h"

namespace DbNodes::Abstract {

    AbstractSettingModal::AbstractSettingModal(QWidget *parent) : AbstractModal(parent)
    {
        // Disabled MainWindow then relation maker open
        mainWindow = Helper::findParentWidgetRecursive(this, "MainWindow");
        mainWindow->setDisabled(true);

        // Enabled this window
        setEnabled(true);

        setStyleSheet(Helper::getStyleFromFile("subWindow"));

        pbStyle = Helper::getStyleFromFile("button");
    }

    void AbstractSettingModal::createSettingGroup(const QString &name)
    {
        auto *hl = new QHBoxLayout();

        auto *title = getTitle(name);
        title->setStyleSheet(title->styleSheet() + "QLabel{font-size: 18px;}");

        hl->addWidget(title);
        hl->addWidget(getLine());

        scrollLayout->addLayout(hl);
    }

    QCheckBox *AbstractSettingModal::createCheckboxSetting(
        const QString &name,
        const QString &settingKey,
        const EVENT_CALLBACK(QCheckBox) &callback
    ) {
        auto *hl = new QHBoxLayout();

        auto *checkBox = new QCheckBox(scrollWidget);

        checkBox->setStyleSheet(
            Helper::getStyleFromFile("settingsCheckBox")
                .arg(Helper::getIconPath("check", false))
        );

        auto state = getDefaultSetting(settingKey).toBool();

        if (state) checkBox->setCheckState(Qt::Checked);

        connect(checkBox, &QCheckBox::stateChanged, this,
            [this, settingKey, checkBox, callback] (const int &state) {
                if (callback != nullptr) callback(checkBox);
                changeSettingMap(settingKey, state);
            }
        );

        settingsMap.insert(settingKey, state);

        hl->addSpacing(settingIndent);
        hl->addWidget(checkBox);
        hl->addWidget(getTitle(name));
        hl->addStretch();

        scrollLayout->addLayout(hl);

        return checkBox;
    }

    QLabel *AbstractSettingModal::getLine()
    {
        auto *line = new QLabel("", scrollWidget);
        line->setFixedHeight(1);
        line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        line->setStyleSheet("background-color: #666666;");

        return line;
    }

    QLabel *AbstractSettingModal::getTitle(const QString &name)
    {
        auto *label = new QLabel(name, scrollWidget);
        label->setFixedHeight(30);
        label->setStyleSheet(Helper::getStyleFromFile("title"));

        return label;
    }

    QLineEdit *AbstractSettingModal::createTextSetting(
        const QString &name,
        const QString &settingKey,
        const EVENT_CALLBACK(QLineEdit) &callback
    ) {
        auto *hl = new QHBoxLayout();

        hl->addSpacing(settingIndent);
        hl->addWidget(getTitle(name));

        auto value = getDefaultSetting(settingKey);

        auto *lineEdit = new QLineEdit(scrollWidget);
        lineEdit->setFixedHeight(30);
        lineEdit->setStyleSheet(Helper::getStyleFromFile("lineEdit lineEditError"));
        lineEdit->setProperty("warning", false);
        lineEdit->setText(value.toString());

        connect(lineEdit, &QLineEdit::textChanged,this,
            [this, settingKey, callback, lineEdit] (const QString &text) {
                if (callback != nullptr) callback(lineEdit);
                changeSettingMap(settingKey, text);
            }
        );

        settingsMap.insert(settingKey, value);

        hl->addWidget(lineEdit);

        scrollLayout->addLayout(hl);

        return lineEdit;
    }

    QSpinBox *AbstractSettingModal::createSpinboxSetting(
        const QString &name,
        const QString &settingKey,
        const QPoint &range,
        const EVENT_CALLBACK(QSpinBox) &callback
    ) {
        auto *hl = new QHBoxLayout();

        hl->addSpacing(settingIndent);
        hl->addWidget(getTitle(name));

        auto value = getDefaultSetting(settingKey).toInt();

        auto *spinBox = new QSpinBox(scrollWidget);
        spinBox->setValue(value);

        spinBox->setStyleSheet(
            Helper::getStyleFromFile("settingSpinBox")
                .arg(Helper::getIconPath("up_arrow", false))
                .arg(Helper::getIconPath("down_arrow", false))
        );

        spinBox->setFixedSize(100, 30);

        if (!range.isNull()) {
            spinBox->setRange(range.x(), range.y());
        }

        spinBox->setValue(value);

        connect(spinBox, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this,
            [this, spinBox, settingKey, callback] (int value) {
                if (callback != nullptr) callback(spinBox);
                changeSettingMap(settingKey, value);
            }
        );

        settingsMap.insert(settingKey, value);

        hl->addStretch();
        hl->addWidget(spinBox);
        hl->addSpacing(100);

        scrollLayout->addLayout(hl);

        return spinBox;
    }

    QComboBox *AbstractSettingModal::createComboBoxSetting(
        const QString &name,
        const QString &settingKey,
        const VARIANTS_MAP &values,
        const EVENT_CALLBACK(QComboBox) &callback
    ) {
        auto *hl = new QHBoxLayout();

        hl->addSpacing(settingIndent);
        hl->addWidget(getTitle(name));

        auto *comboBox = new QComboBox(scrollWidget);

        comboBox->setStyleSheet(
            Helper::getStyleFromFile("settingsComboBox")
                .arg(Helper::getIconPath("down_arrow", false))
        );

        comboBox->setFixedSize(200, 30);

        foreach (const QString &key, values.keys()) {
            comboBox->addItem(values.value(key).toString(), key);
        }

        auto value = getDefaultSetting(settingKey);

        comboBox->setCurrentIndex(comboBox->findData(value));

        connect(comboBox, &QComboBox::currentTextChanged, this,
            [this, settingKey, comboBox, callback] (const QString &) {
                if (callback != nullptr) callback(comboBox);
                changeSettingMap(settingKey, comboBox->currentData());
            }
        );

        settingsMap.insert(settingKey, value);

        hl->addStretch();
        hl->addWidget(comboBox);
        hl->addSpacing(100);

        scrollLayout->addLayout(hl);

        return comboBox;
    }

    void AbstractSettingModal::exit()
    {
        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(false);
        AbstractModal::exit();
    }

    void AbstractSettingModal::changeSettingMap(const QString &name, const QVariant &value)
    {
        newSettingsMap.insert(name, value);

        if (!applySettings) {
            return;
        }

        foreach (const QString &key, settingsMap.keys()) {
            auto oldValue = settingsMap.value(key);

            if (oldValue != newSettingsMap.value(key)) {
                enableConfirm(true);
                return;
            }

            enableConfirm(false);
        }
    }

    void AbstractSettingModal::accept(const bool &close)
    {
        AbstractModal::accept(close);

        enableConfirm(false);
        settingsMap.clear();
        settingsMap = newSettingsMap;
    }

    void AbstractSettingModal::confirm()
    {
        if (!canSave) return;

        AbstractModal::confirm();
    }

    void AbstractSettingModal::enableConfirm(const bool &enable)
    {
        canSave = enable;
    }

    void AbstractSettingModal::initUI()
    {
        auto *widget = new QWidget(this);
        auto *layout = new QVBoxLayout(widget);

        auto *scrollArea = new QScrollArea(this);

        scrollWidget = new QWidget(scrollArea);
        scrollWidget->setFixedWidth(this->width() - 40);
        scrollLayout = new QVBoxLayout(scrollWidget);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->addStretch();
        buttonsLayout->setSpacing(10);

        initSettingsUi();

        scrollWidget->setLayout(scrollLayout);

        scrollArea->setWidget(scrollWidget);
        layout->addWidget(scrollArea);
        layout->addStretch();

        scrollLayout->setSpacing(15);
        scrollLayout->addStretch();

        layout->addWidget(getLine());

        layout->addLayout(buttonsLayout);

        widget->setLayout(layout);
        setCentralWidget(widget);
    }

    void AbstractSettingModal::setSpacing(int size)
    {
        scrollLayout->setSpacing(size);
    }

    void AbstractSettingModal::addStretch()
    {
        scrollLayout->addStretch();
    }

    QVariant AbstractSettingModal::getDefaultSetting(const QString &name)
    {
        return Helper::getSettingValue(name);
    }

    void AbstractSettingModal::prepareWidget()
    {
        beforeInitUi();

        initUI();

        afterInitUi();

        newSettingsMap = settingsMap;

        mainWindow = Helper::findParentWidgetRecursive(this, "MainWindow");
        Helper::moveToCenter(mainWindow, this);
    }

    void AbstractSettingModal::setSettingIndent(const int &size)
    {
        settingIndent = size;
    }

    QPushButton *AbstractSettingModal::createButton(const QString &name, QObject *target, const std::function<void()> &f)
    {
        auto *button = new QPushButton(name, this);
        button->setFixedSize(70, 30);
        button->setStyleSheet(pbStyle);

        buttonsLayout->addWidget(button);

        connect(button, &QPushButton::clicked, target, f);

        return button;
    }

    void AbstractSettingModal::bindApplySettings(const bool &enable)
    {
        applySettings = enable;
    }

    void AbstractSettingModal::beforeInitUi() {}

    void AbstractSettingModal::afterInitUi() {}

    void AbstractSettingModal::initSettingsUi() {}
}