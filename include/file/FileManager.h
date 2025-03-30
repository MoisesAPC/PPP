#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/******************************************************************************
 * FileManager.h
 *
 * Header file for FileManager.cpp
 *****************************************************************************/

#include "include/file/FileLoader.h"
#include <QFile>
#include <QtEndian>
#include <QFileInfo>

class FileManager {
    public:
        // The file format of the opened file
        enum eFormat {
            FORMAT_NOTE,                  // .note
            FORMAT_CONTROLLERPAK,         // .pak, .mpk
            FORMAT_CARTRIDGE,             // .eep
            FORMAT_DEXDRIVE               // .n64, .t64
        };

        struct ControllerPakIndexData {
            int index = -1;
            short region = SaveData::USA;
            unsigned int rawDataStartOffset = 0;

            ControllerPakIndexData() { clearEntry(); }

            void clearEntry() {
                index = -1;
                region = SaveData::USA;
                rawDataStartOffset = 0;
            }
        };

        const unsigned int CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE = 0x20;
        const unsigned int CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES = 16;

        static FileManager* getInstance() {
            if (instance == nullptr) {
                createInstance();
            }

            return instance;
        }

        static void createInstance() {
            instance = new FileManager();
        }

        static void destroyInstance() {
            delete instance;
            instance = nullptr;
        }

        int getFileFormat() const {
            return format;
        }

        void setFileFormat(const int format_) {
            format = format_;
        }

        QString& getFilepath() {
            return filepath;
        }

        void setFilePath(const QString& filepath_) {
            filepath = filepath_;
        }

        QFile& getFile() {
            return *file;
        }

        QByteArray& getBuffer() {
            return *buffer;
        }

        FileLoader* getLoader() {
            return loader;
        }

        int getControllerPakCurrentlySelectedSaveIndex() const {
            return controllerPakCurrentlySelectedSaveIndex;
        }

        void setControllerPakCurrentlySelectedSaveIndex(int controllerPakCurrentlySelectedSaveIndex_) {
            controllerPakCurrentlySelectedSaveIndex = controllerPakCurrentlySelectedSaveIndex_;
        }

        int openFile(const QString& filepath_);
        int writeFile(const QString& filepath_);

        std::vector<ControllerPakIndexData>* getControllerPakIndexDataArray() {
            return &indexDataArray;
        }

        unsigned int initIndexData(QFile& file);

        void clearIndexData() {
            for (int i = 0; i < indexDataArray.size(); i++) {
                indexDataArray[i].clearEntry();
            }
        }

        bool wasFileOpened() const {
            return fileOpened;
        }

        void setFileOpened(const bool fileOpened_) {
            fileOpened = fileOpened_;
        }

    private:
        static FileManager* instance;

        FileManager() {
            filepath = "";
            buffer = new QByteArray();
        }

        ~FileManager() {
            if (file != nullptr) {
                file->close();

                delete file;
                file = nullptr;
            }

            if (loader != nullptr) {
                delete loader;
                loader = nullptr;
            }

            if (buffer != nullptr) {
                delete buffer;
                buffer = nullptr;
            }
        }

        FileManager(const FileManager& obj) = delete; // Remove the copy constructor
        int determineFormat();

        int format = FORMAT_NOTE;
        int controllerPakCurrentlySelectedSaveIndex = 0;    // The index of the currently selected save in a loaded Controller Pak

        QFile* file = nullptr;
        QByteArray* buffer = nullptr;
        QString filepath;
        FileLoader* loader = nullptr;
        bool fileOpened = false;    // A file was opened at least once. Used for knowing if we have to enable or disable the Save buttons

        std::vector<ControllerPakIndexData> indexDataArray{CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES};
};

#endif
