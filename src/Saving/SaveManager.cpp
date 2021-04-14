//
// Created by artem on 10.03.2021.
//

#include "QJsonDocument"
#include "QRegExp"

#include "SaveManager.h"
#include "QString"
#include "QFileDialog"
#include "QFile"
#include "QDebug"
#include "QPair"

#include "ExceptionModal.h"

namespace DbNodes::Saving {

    SaveManager::SaveManager(QWidget *parent): QObject(parent) {}

    QPair<QString, QByteArray> SaveManager::openFile()
    {
        auto filePath = QFileDialog::getOpenFileName(
            nullptr,
            tr("Open File"),
            "/home/" + qgetenv("USER") + "/file.dbn",
            tr("DbNodes File (*.dbn)")
        );

        if (filePath == "") {
            return QPair<QString, QByteArray>();
        }

        return QPair<QString, QByteArray>(filePath, readFile(filePath));
    }

    QPair<QString, QByteArray> SaveManager::getFileContent(const QString &path)
    {
        if (path == nullptr) {
            auto pair = openFile();
            lastOpenedPath = pair.first;
            return pair;
        }

        lastOpenedPath = path;
        return QPair<QString, QByteArray>(path, readFile(path));
    }

    QString SaveManager::createNewFile(const QString &path)
    {
        if (path == nullptr) {
            return QFileDialog::getSaveFileName(
                nullptr,
                tr("Save File"),
                "/home/" + qgetenv("USER") + "/new_file.dbn",
                tr("DbNodes File (*.dbn)")
            );
        } else {
            QFile file(path);

            if (file.exists()) return path;

            createDirsInPath(path);

            file.open(QIODevice::WriteOnly);
            file.close();
        }

        return path;
    }

    void SaveManager::setFileContent(const QString &path, const QByteArray &content)
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            generateException(Dictionaries::OpenFileExceptionsDictionary::Type::CanNotOpenFile, path);

        file.write(content);
        file.close();
    }

    QString SaveManager::getLastOpenFilePath()
    {
        return lastOpenedPath;
    }

    bool SaveManager::fileExists(const QString &path)
    {
        QFile file(path);

        return file.exists();
    }

    void SaveManager::createDirsInPath(const QString &path)
    {
        auto folders = path.split("/");

        if (QRegExp("\\w+.\\w+").exactMatch(folders.last())) {
            folders.removeLast();
        }

        foreach (const QString &folder, folders) {
            if (folder == "") continue;

            auto currentPath = QDir(path.split(folder)[0]);

            if (!currentPath.exists(folder)) {
                currentPath.mkdir(folder);
            }
        }
    }

    QByteArray SaveManager::readFile(const QString &path)
    {
        QByteArray fileString("");
        QFile file(path);

        if (!file.exists()) {
            return generateException(Dictionaries::OpenFileExceptionsDictionary::Type::FileNotFound, path);
        }

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return generateException(Dictionaries::OpenFileExceptionsDictionary::Type::CanNotOpenFile, path);
        }

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            fileString += line;
        }

        file.close();

        if (fileString.isEmpty()) {
            return generateException(Dictionaries::OpenFileExceptionsDictionary::Type::FileIsEmpty);
        }

        if (QJsonDocument::fromJson(fileString).isEmpty()) {
            return generateException(Dictionaries::OpenFileExceptionsDictionary::Type::InvalidFileType);
        }

        return fileString;
    }

    QByteArray SaveManager::generateException(
        const Dictionaries::OpenFileExceptionsDictionary::Type &exceptionType,
        const QString &path
    ) {
        QString exceptionText = Dictionaries::OpenFileExceptionsDictionary::getValue(exceptionType).toString();

        if (exceptionText.indexOf("%1") > 0)
            exceptionText = exceptionText.arg(path);

        new Modals::ExceptionModal(
            "Open file",
            exceptionText,
            dynamic_cast<QWidget *>(this->parent())
        );

        return "";
    }
}