#ifndef HELPER_H
#define HELPER_H

#include "QString"
#include "QWidget"

class Helper
{
    public:
        static QString  getStyleFromFile(const QString &name);
        static QString  getfileContent(const QString &path);
        static QString  getCurrentTimeMS();
        static QObject *findParentObjectRecursive(QObject *object, const QString &name);
        static QWidget *findParentWidgetRecursive(QWidget *widget, const QString &name);
        static QString  getIconPath(const QString &iconName, const bool &styled = true);
};

#endif // HELPER_H
