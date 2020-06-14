#include "helper.h"
#include "qfile.h"
#include "qpointer.h"
#include "noderow.h"
#include "QDateTime"
#include "QDebug"

QString Helper::getStyleFromFile(const QString name) {
    QFile file(":/styles/" + name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString string;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        string += line;
    }

    return string;
}

template <typename T>
QString Helper::pointToString(T ptr)
{
    return QString("0x%1").arg((quintptr)ptr, QT_POINTER_SIZE * 2, 16, QChar('0'));
}
template QString Helper::pointToString<NodeRow*>(NodeRow*);

QString Helper::getCurrentTimeMS()
{
    return QString::number(QDateTime::currentMSecsSinceEpoch());
}

