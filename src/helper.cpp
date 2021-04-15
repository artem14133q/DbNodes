#include "QFile"
#include "QDateTime"
#include "QDebug"
#include "QStringList"
#include "QDir"

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

void Helper::subscribeSettingUpdate(const QString &key, const MainSettings::ConnectorCallback &callback)
{
    MainSettings::getInstance()->subscribe(key.split(".").join("/"), callback);
}

void Helper::unBindSetting(const QString &key)
{
    MainSettings::getInstance()->unBind(key.split(".").join("/"));
}

void Helper::moveToCenter(QWidget *mapToWidget, QWidget *currentWidget)
{
    currentWidget->move(
        mapToWidget->x() + mapToWidget->width() / 2 - currentWidget->width() / 2,
        mapToWidget->y() + mapToWidget->height() / 2 - currentWidget->height() / 2
    );
}

QString Helper::replaceHomePathInFullPath(const QString &path, const QString &replace)
{
    return QString(path).replace(QDir::home().path(), replace);
}

void Helper::standardWidgetsHeight(const int &height, QWidget *parent, const QStringList &names, const bool &exclude)
{
    foreach (QWidget *widget, parent->findChildren<QWidget *>()) {
        if (!names.isEmpty()) {
            if (names.contains(widget->objectName())) {
                if (exclude) continue;
            } else {
                if (!exclude) continue;
            }
        }

        widget->setFixedHeight(height);
    }

    parent->adjustSize();
}

void Helper::updateParentRecursive(QWidget *currentWidget, const QString &name)
{
    auto workArea = Helper::findParentWidgetRecursive(currentWidget, name);
    workArea->update();
}

