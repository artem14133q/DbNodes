//
// Created by artem on 26.12.2020.
//

#include "TableConstructor.h"
#include "QString"
#include "QLabel"
#include "QLineEdit"

namespace DbNodes::Modals {

    TableConstructor::TableConstructor(const QString &name, QWidget *parent)
        : Abstract::AbstractSettingModal(parent), tableName(name)
    {
        setFixedWidth(500);
        setWindowTitle(QString("Rename table: %1").arg(name));

        prepareWidget();

        show();
    }

    void TableConstructor::initSettingsUi()
    {
        nameLineEdit = createTextSetting("Name", "name");

        setSpacing(15);

        createSpinboxSetting("Foreign keys", "foreignCount", {0, 999});
        createSpinboxSetting("Columns", "defaultCount", {0, 999});

        createCheckboxSetting("Add id column", "addId");

        // Create buttons

        createButton("Cancel", this, [this] {
            emit pushExit();
            exit();
        });

        createButton("Ok", this, [this] {
            confirm();
        });
    }

    QVariant TableConstructor::getDefaultSetting(const QString &name)
    {
        return tableName;
    }

    void TableConstructor::confirm()
    {
        emit pushConfirm(newSettingsMap);

        enableConfirm(true);
        Abstract::AbstractSettingModal::confirm();
    }

    void TableConstructor::afterInitUi()
    {
        nameLineEdit->setFocus();
        nameLineEdit->selectAll();
    }
}