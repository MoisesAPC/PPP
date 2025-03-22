#include "include/file/FileManager.h"
#include "include/save/SaveManager.h"
#include "include\windows\controllerpakselection\controllerpakselectionwindow.h"

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
        else if (fileExtension == "mpk" || fileExtension == "pak") {
            loader = new FileLoaderControllerPak();
            format = FORMAT_CONTROLLERPAK;
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

                // Initialize Controller Pak specific data
                if (format == FORMAT_CONTROLLERPAK) {
                    unsigned int numCV64Saves = loader->initIndexData(*file);

                    // Stop opening the file if the Controller Pak doesn't have any Castlevania saves
                    // previously stored on it
                    if (numCV64Saves == 0) {
                        buffer->clear();
                        buffer->resize(0);
                        file->close();
                        return;
                    }

                    // Open the selection window with the gathered Castlevania 64 saves
                    ControllerPakSelectionWindow* PakSaveSelectWindow = new ControllerPakSelectionWindow();
                    int result = PakSaveSelectWindow->exec();

                    // Return early if the user clicked on the X instead of on a button
                    if (result == QDialog::Rejected) {
                        buffer->clear();
                        buffer->resize(0);
                        file->close();
                        return;
                    }
                }

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
