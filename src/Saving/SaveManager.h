//
// Created by artem on 10.03.2021.
//

#ifndef DBNODES_SAVEMANAGER_H
#define DBNODES_SAVEMANAGER_H

#include "QObject"
#include "QHash"

namespace DbNodes::Saving {

    class SaveManager: public QObject
    {
            QHash<QString, QByteArray> files;

        public:
            explicit SaveManager(QWidget *parent = nullptr);
            Q_DISABLE_COPY(SaveManager)

            void closeSaveFile(const QString &path);

            QString openFile(int &exception);

            static QString createNewFile();

            QByteArray getFileContent(const QString &path = nullptr);
            void setFileContent(const QString &path, const QByteArray &content);

            void saveFile(const QString &path);

            QString getLastOpenFilePath();

            ~SaveManager() override;
    };

}

#endif //DBNODES_SAVEMANAGER_H
