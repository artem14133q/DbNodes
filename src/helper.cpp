#include "QFile"
#include "QDateTime"
#include "QDebug"
#include "QStringList"

#include "helper.h"
#include "config.h"

QString Helper::getStyleFromFile(const QString &name) {
    using namespace DbNodes::Settings;

    auto nameList = name.split(" ");
    auto style = getSettingValue("style.stylesheet_folder").toString();

    QString content;

    foreach (const QString &fileName, nameList) {
        content += getfileContent(QStringList({STYLE_FILES_PATH, style, fileName}).join("/") + ".qss");
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
        auto style = getSettingValue("style.icons_folder").toString();
        return QStringList({ICONS_FILES_PATH, style, iconName}).join("/") + ".png";
    }

    return QStringList({ICONS_FILES_PATH, iconName}).join("/") + ".png";
}

QVariant Helper::getSettingValue(const QString &name)
{
    return MainSettings::get(name.split(".").join("/")).toString();
}

void Helper::setSettingValue(const QString &name, const QVariant &value)
{
    MainSettings::set(name.split(".").join("/"), value);
}

void Helper::subscribeSettingUpdate(const QString &key, const CONNECTOR_CALLBACK &callback)
{
    MainSettings::getInstance()->subscribe(key.split(".").join("/"), callback);
}

void Helper::unBindSetting(const QString &key)
{
    MainSettings::getInstance()->unBind(key.split(".").join("/"));
}

