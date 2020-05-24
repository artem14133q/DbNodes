#include "helper.h"
#include "qfile.h"
#include "qpointer.h"
#include "noderow.h"

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
