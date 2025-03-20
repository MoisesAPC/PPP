#include "include/file/FileManager.h"
#include "include/save/SaveManager.h"

void FileManager::determineFormat() {
    if (!filepath.isEmpty()) {

        if (loader != nullptr) {
            delete loader;
            loader = nullptr;
        }

        QFileInfo fileInfo(filepath);

        QString fileExtension = fileInfo.suffix();

        if (fileExtension == "note") {
            loader = new FileLoaderNote();
            format = FORMAT_NOTE;
        }
    }
}

void FileManager::openFile(const QString& filepath_) {
    if (!filepath_.isEmpty()) {
        setFilePath(filepath_);
        determineFormat();
        file = new QFile(filepath);

        if (file->open(QIODevice::ReadOnly)) {
            if (loader != nullptr) {
                loader->parseRegion(*file);
                loader->readAllSaveSlots(*file);
            }

        }

        file->close();
    }
}

void FileManager::writeFile(const QString& filepath_) {
    if (!filepath_.isEmpty()) {
        setFilePath(filepath_);
        determineFormat();
        file = new QFile(filepath);

        if (file->open(QIODevice::WriteOnly)) {
            if (loader != nullptr) {
                loader->writeAllSaveSlots(*file);
            }

        }

        file->close();
    }
}
