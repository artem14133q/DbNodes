//
// Created by artem on 10.03.2021.
//

#include "QJsonDocument"
#include "SaveManager.h"

#include "QString"
#include "QFileDialog"
#include "QFile"

#include "OpenFileExceptionsDictionary.h"
#include "ExceptionModal.h"

namespace DbNodes::Saving {

    SaveManager::SaveManager(QWidget *parent): QObject(parent) {}

    QString SaveManager::openFile(int &exception)
    {
        auto filePath = QFileDialog::getOpenFileName(
            nullptr,
            tr("Open File"),
            "/home/" + qgetenv("USER") + "/file.dbn",
            tr("DbNodes File (*.dbn)")
        );

        if (filePath == "") {
            exception = FILE_PATH_IS_EMPTY_EXCEPTION;
            return "";
        }

        QByteArray fileString("");
        QFile file(filePath);

        if (!file.exists()) {
            exception = FILE_NOT_FOUND_EXCEPTION;
            return filePath;
        }

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            exception = CAN_NOT_OPEN_FILE_EXCEPTION;
            return filePath;
        }

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            fileString += line;
        }

        file.close();

        if (fileString.isEmpty()) {
            exception = FILE_IS_EMPTY_EXCEPTION;
            return filePath;
        }

        if (QJsonDocument::fromJson(fileString).isEmpty()) {
            exception = INVALID_FILE_TYPE_EXCEPTION;
            return filePath;
        }

        files.insert(filePath, fileString);

        return filePath;
    }

    void SaveManager::closeSaveFile(const QString &path)
    {
        files.remove(path);
    }

    QByteArray SaveManager::getFileContent(const QString &path)
    {
        int exception = 0;

        QString filePath(path == nullptr ? openFile(exception) : path);

        if (exception > 0) {
            QString exceptionText = Dictionaries::OpenFileExceptionsDictionary::getValue(exception).toString();

            if (exceptionText.indexOf("%1") > 0)
                exceptionText = exceptionText.arg(filePath);

            new Modals::ExceptionModal(
                "Open file",
                exceptionText,
                dynamic_cast<QWidget *>(this->parent())
            );

            return QByteArray();
        }

        return files.value(filePath);
    }

    QString SaveManager::createNewFile()
    {
        auto path = QFileDialog::getSaveFileName(
            nullptr,
            tr("Save File"),
            "/home/" + qgetenv("USER") + "/new_file.dbn",
            tr("DbNodes File (*.dbn)")
        );

        return path;
    }

    SaveManager::~SaveManager()
    {
        files.clear();

        QObject::deleteLater();
    }

    void SaveManager::setFileContent(const QString &path, const QByteArray &content)
    {
        files.insert(path, content);

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            new Modals::ExceptionModal(
                "Open file",
                Dictionaries::OpenFileExceptionsDictionary::getValue(
                    CAN_NOT_OPEN_FILE_EXCEPTION
                ).toString().arg(path),
                dynamic_cast<QWidget *>(this->parent())
            );

        file.write(content);
        file.close();
    }

    void SaveManager::saveFile(const QString &path)
    {
        setFileContent(path, files.value(path));
    }

    QString SaveManager::getLastOpenFilePath()
    {
        return files.keys().last();
    }

}