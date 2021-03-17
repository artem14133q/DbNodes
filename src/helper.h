#ifndef HELPER_H
#define HELPER_H

#include "QString"
#include "QWidget"

#include "MainSettings.h"

using namespace DbNodes::Settings;

class Helper
{
    public:
        static QString  getStyleFromFile(const QString &name);
        static QString  getfileContent(const QString &path);
        static QString  getCurrentTimeMS();
        static QObject *findParentObjectRecursive(QObject *object, const QString &name);
        static QWidget *findParentWidgetRecursive(QWidget *widget, const QString &name);
        static QString  getIconPath(const QString &iconName, const bool &styled = true);
        static QVariant getSettingValue(const QString &name);
        static void setSettingValue(const QString &name, const QVariant &value);
        static void subscribeSettingUpdate(const QString &key, const CONNECTOR_CALLBACK &callback);
        static void unBindSetting(const QString &key);
        static void moveToCenter(QWidget *mapToWidget, QWidget *currentWidget);
};

#endif // HELPER_H
