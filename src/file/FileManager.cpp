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
        else if (fileExtension == "eep") {
            loader = new FileLoaderCartridge();
            format = FORMAT_CARTRIDGE;
        }
    }
}

void FileManager::openFile(const QString& filepath_) {
    if (!filepath_.isEmpty()) {
        setFilePath(filepath_);
        determineFormat();
        file = new QFile(filepath);

        if (file->open(QIODevice::ReadOnly)) {
            // First, write to the buffer
            QDataStream inputStream(file);

            QByteArray fileData = file->readAll();
            *buffer = file->readAll();
            buffer->resize(fileData.size());
            *buffer = fileData;

            // Then, parse the contents of the file
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

        if (file->open(QIODevice::ReadWrite)) {
            // Copy the contents of the file buffer containing the previously unsaved data.
            // Then, overwrite with the new data.
            file->write(*buffer);

            if (loader != nullptr) {
                loader->writeAllSaveSlots(*file);
            }
        }

        file->close();
    }
}
