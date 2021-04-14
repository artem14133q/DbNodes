//
// Created by artem on 10.03.2021.
//

#ifndef DBNODES_SAVEMANAGER_H
#define DBNODES_SAVEMANAGER_H

#include "QObject"
#include "QHash"

#include "OpenFileExceptionsDictionary.h"

namespace DbNodes::Saving {

    class SaveManager: public QObject
    {
        private:
            QByteArray readFile(const QString &path);

            QByteArray generateException(
                const Dictionaries::OpenFileExceptionsDictionary::Type &exceptionType,
                const QString &path = nullptr
            );

            QString lastOpenedPath = "";

        public:
            explicit SaveManager(QWidget *parent = nullptr);
            Q_DISABLE_COPY(SaveManager)

            QPair<QString, QByteArray> openFile();

            static QString createNewFile(const QString &path = nullptr);

            QPair<QString, QByteArray> getFileContent(const QString &path = nullptr);
            void setFileContent(const QString &path, const QByteArray &content);

            static void createDirsInPath(const QString &path);

            static bool fileExists(const QString &path);

            QString getLastOpenFilePath();
    };

}

#endif //DBNODES_SAVEMANAGER_H
