#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QPointer>
#include <noderow.h>

class Helper
{
public:
    static QString getStyleFromFile(const QString);

    template <typename T>
    static QString pointToString(T);
};

#endif // HELPER_H
