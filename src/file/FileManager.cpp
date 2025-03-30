#include "include/file/FileManager.h"
#include "include/save/SaveManager.h"
#include "include/windows/ControllerPakSelection/ControllerPakSelectionwindow.h"
#include <QMessageBox>

int FileManager::determineFormat() {
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
        else if (fileExtension == "n64" || fileExtension == "t64") {
            loader = new FileLoaderDexDrive();
            format = FORMAT_DEXDRIVE;
        }
        else {
            // Unsupported file
            return -1;
        }

        return 0;
    }

    return -1;
}

int FileManager::openFile(const QString& filepath_) {
    if (!filepath_.isEmpty()) {
        setFilePath(filepath_);

        if (determineFormat() == -1)  {
            return -1;
        }

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
                if (format == FORMAT_CONTROLLERPAK || format == FORMAT_DEXDRIVE) {
                    unsigned int numCV64Saves = initIndexData(*file);

                    // Stop opening the file if the Controller Pak doesn't have any Castlevania saves
                    // previously stored on it
                    if (numCV64Saves == 0) {
                        buffer->clear();
                        buffer->resize(0);
                        file->close();

                        QMessageBox::critical(nullptr, "Error", "This file doesn't have any active, valid saves.");
                        return -1;
                    }

                    // Open the selection window with the gathered Castlevania 64 saves
                    ControllerPakSelectionWindow* PakSaveSelectWindow = new ControllerPakSelectionWindow();
                    int result = PakSaveSelectWindow->exec();

                    // Return early if the user clicked on the X instead of on a button
                    if (result == QDialog::Rejected) {
                        buffer->clear();
                        buffer->resize(0);
                        file->close();
                        return -2;
                    }
                }

                loader->parseRegion(*file);
                loader->readAllSaveSlots(*file);

                if (fileOpened == false) {
                    fileOpened = true;
                }
            }
        }
        else {
            return -1;
        }

        file->close();
    }

    return 0;
}

int FileManager::writeFile(const QString& filepath_) {
    if (SaveManager::getInstance()->areAllSavesDisabled()) {
        return -2;
    }

    if (!filepath_.isEmpty()) {
        setFilePath(filepath_);

        if (determineFormat() == -1)  {
            return -1;
        }

        file = new QFile(filepath);

        if (file->open(QIODevice::ReadWrite)) {
            // Copy the contents of the file buffer containing the previously unsaved data.
            // Then, overwrite with the new data.
            file->write(*buffer);

            if (loader != nullptr) {
                loader->writeAllSaveSlots(*file);
            }
        }
        else {
            return -1;
        }

        file->close();

        return 0;
    }

    return -1;
}

// Initialize the FileManager's "indexDataArray", in order to know extra information regarding each Castlevania save it has
unsigned int FileManager::initIndexData(QFile& file) {
    unsigned int numCV64Saves = 0;

    if (loader != nullptr && (format == FORMAT_CONTROLLERPAK || format == FORMAT_DEXDRIVE)) {
        SaveManager* saveManager = SaveManager::getInstance();
        QDataStream inputStream(&file);
        std::vector<FileManager::ControllerPakIndexData>* indexDataArray = FileManager::getInstance()->getControllerPakIndexDataArray();

        // If opening another Controller Pak file, make sure to clear the index data array first
        clearIndexData();

        for (int i = 0; i < loader->getNoteTableNumEntries(); i++) {
            const unsigned int GAMEID_SIZE = 6;
            QByteArray gameId(GAMEID_SIZE, '\0');

            inputStream.device()->seek(loader->getNoteTableOffset() + (loader->getNoteTableEntrySize() * i));   // This is where the current entry data starts
            unsigned int bytesRead = inputStream.readRawData(gameId.data(), GAMEID_SIZE);
            inputStream.device()->seek(loader->getNoteTableOffset() + (loader->getNoteTableEntrySize() * i));   // Go back to where we were previously to reading gameId

            if (bytesRead != GAMEID_SIZE) {
                return 0;
            }

            // If the game save stored doesn't belong to any of the Castlevania 64 versions, skip to the next save
            if (gameId != "ND3EA4" && gameId != "ND3PA4" && gameId != "ND3JA4") {
                continue;
            }

            // Get the save index within the Controller Pak data
            (*indexDataArray)[i].index = i;

            // Parse the region (at offset +3)
            unsigned char regionFromFile = loader->readData<unsigned char>(inputStream, inputStream.device()->pos() + 3);
            (*indexDataArray)[i].region = loader->getRegionEnumFromChar(regionFromFile);

            // Parse the raw data start offset (at offset +3 after the region, and then multiplied by 0x100)
            // If this is 0, we skip over this save entry (acting as if it wasn't present), and go to the next one
            unsigned int rawDataStartOffsetByte = loader->readData<unsigned char>(inputStream, inputStream.device()->pos() + 3);
            if (rawDataStartOffsetByte == 0) {
                (*indexDataArray)[i].clearEntry();
                numCV64Saves--;
                continue;
            }
            (*indexDataArray)[i].rawDataStartOffset = loader->getRawDataOffsetPerEntry(rawDataStartOffsetByte);

            numCV64Saves++;
        }
    }

    return numCV64Saves;
}
