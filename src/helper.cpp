#include "QFile"
#include "QDateTime"
#include "QDebug"
#include "QStringList"

#include "helper.h"
#include "SettingsManager.h"
#include "config.h"

QString Helper::getStyleFromFile(const QString &name) {
    auto nameList = name.split(" ");
    auto style = DbNodes::Settings::SettingsManager::getSetting("style");

    QString content;

    foreach (QString fileName, nameList) {
        content += getfileContent(QStringList({STYLE_FILES_PATH, style, name}).join("/") + ".qss");
    }

    return content;
}

QString Helper::getCurrentTimeMS()
{
    return QString::number(QDateTime::currentMSecsSinceEpoch());
}

QObject *Helper::findParentObjectRecursive(QObject *object, const QString &name)
{
    QObject *parent = object;

    do {
        try {
            parent = parent->parent();
        } catch (...) {
            return nullptr;
        }
    } while (parent->objectName() != name);

    return parent;
}

QWidget *Helper::findParentWidgetRecursive(QWidget *widget, const QString &name)
{
    QWidget *parent = widget;

    do {
        try {
            parent = parent->parentWidget();
        } catch (...) {
            return nullptr;
        }
    } while (parent->objectName() != name);

    return parent;
}

QString Helper::getfileContent(const QString &path)
{
    QString content;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        content += line;
    }

    file.close();

    return content;
}

QString Helper::getIconPath(const QString &iconName, const bool &styled)
{
    if (styled) {
        auto style = DbNodes::Settings::SettingsManager::getSetting("icons_style");
        return QStringList({ICONS_FILES_PATH, style, iconName}).join("/") + ".png";
    }

    return QStringList({ICONS_FILES_PATH, iconName}).join("/") + ".png";
}
