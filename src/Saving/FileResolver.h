//
// Created by artem on 11.03.2021.
//

#ifndef DBNODES_FILERESOLVER_H
#define DBNODES_FILERESOLVER_H

#include "QObject"

#include "SaveManager.h"

namespace DbNodes::Saving {

    template<typename T>
    class FileResolver: public QObject
    {
        private:
            SaveManager *saveManager;

        protected:
            T *object = nullptr;

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

            bool load(const QString &path = nullptr)
            {
                auto fileBytes = saveManager->getFileContent(path);

                if (fileBytes.isEmpty()) {
                    return false;
                }

                object->setBaseObject(QJsonDocument().fromJson(fileBytes).object());

                fromJson();

                return true;
            }

            ~FileResolver() override
            {
                delete object;

                deleteLater();
            }
    };

}

#endif //DBNODES_FILERESOLVER_H
