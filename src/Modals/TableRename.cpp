//
// Created by artem on 26.12.2020.
//

#include "TableRename.h"
#include "QString"
#include "QLabel"
#include "QLineEdit"

namespace DbNodes::Modals {

    TableRename::TableRename(const QString &name, QWidget *parent)
        : Abstract::AbstractSettingModal(parent), tableName(name)
    {
        setFixedWidth(500);
        setWindowTitle(QString("Rename table: %1").arg(name));

        prepareWidget();

        show();
    }

    void TableRename::initSettingsUi()
    {
        nameLineEdit = createTextSetting("Name:", "name");

        // Create buttons

        createButton("Cancel", this, [this] {
            exit();
        });

        createButton("Ok", this, [this] {
            confirm();
        });
    }

    QVariant TableRename::getDefaultSetting(const QString &name)
    {
        return tableName;
    }

    void TableRename::confirm()
    {
        emit pushConfirm(newSettingsMap.value("name").toString());

        enableConfirm(true);
        Abstract::AbstractSettingModal::confirm();
    }

    void TableRename::afterInitUi()
    {
        nameLineEdit->setFocus();
        nameLineEdit->selectAll();
    }
}