#include "QFile"
#include "QDateTime"
#include "QDebug"
#include "QStringList"

#include "helper.h"
#include "SettingsManager.h"

QString Helper::getStyleFromFile(const QString &name) {
    auto nameList = name.split(" ");
    auto style = DbNodes::Settings::SettingsManager::getSetting("style");

    QString content;

    foreach (QString fileName, nameList) {
        content += getfileContent(QStringList({":/styles", style, name}).join("."));
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

QString Helper::getfileContent(const QString &dotName)
{
    QString content;

    auto nameArr = dotName.split(".");

    QFile file(nameArr.join("/"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        content += line;
    }

    return content;
}

QString Helper::getIconPath(const QString &iconName)
{
    auto style = DbNodes::Settings::SettingsManager::getSetting("icons_style");

    return QStringList({":/imgs", style, iconName}).join("/");
}
