//
// Created by artem on 11.03.2021.
//

#ifndef DBNODES_FILERESOLVER_H
#define DBNODES_FILERESOLVER_H

#include "QObject"
#include "QDebug"

#include "SaveManager.h"

namespace DbNodes::Saving {

    template<typename T>
    class FileResolver: public QObject
    {
        protected:
            T *object = nullptr;
            SaveManager *saveManager;

            QString currentFilePath = "";

        public:
            Q_DISABLE_COPY(FileResolver)

            explicit FileResolver(SaveManager *saveManager): saveManager(saveManager)
            {
                object = new T();
            };

            virtual QByteArray toJson()
            {
                return QByteArray();
            }

            virtual void fromJson() {}

            void save(const QString &path)
            {
                saveManager->setFileContent(path, toJson());
            }

            virtual bool load(const QString &path = nullptr)
            {
                auto pair = saveManager->getFileContent(path);

                if (pair.second.isEmpty()) {
                    return false;
                }

                object->setBaseObject(QJsonDocument().fromJson(pair.second).object());

                fromJson();

                currentFilePath = (path == nullptr) ? pair.first : path;

                return true;
            }

            virtual bool createFileIfNotExists(const QString &path)
            {
                if (!SaveManager::fileExists(path)) {
                    SaveManager::createNewFile(path);
                    return true;
                }

                return false;
            }

            [[nodiscard]] QString getCurrentFilePath() const
            {
                return currentFilePath;
            }

            ~FileResolver() override
            {
                delete object;

                deleteLater();
            }
    };

}

#endif //DBNODES_FILERESOLVER_H
