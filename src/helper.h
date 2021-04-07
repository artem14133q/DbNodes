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
        static QString replaceHomePathInFullPath(const QString &path, const QString &replace = "~");
//        static QString getIconFullPath(const QString &iconName, const bool &styled = true);
        static void standardWidgetsHeight(
            const int &height,
            QWidget *parent,
            const QStringList &names = {},
            const bool &exclude = false
        );

        template<class T>
        static void removeDeletedItems(QList<QPointer<T>> &list)
        {
            foreach (const QPointer<T> &item, list) {
                if (!item) list.removeOne(item);
            }
        }
};

#endif // HELPER_H
