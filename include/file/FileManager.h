#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "include/file/FileLoader.h"
#include <QFile>
#include <QtEndian>
#include <QFileInfo>

class FileManager {

    // The file format of the opened file
    // TODO: Add DexDrive (.n64) and T64 save support
    enum eFormat {
        FORMAT_NOTE,                  // .note
        FORMAT_CONTROLLERPAK,         // .pak, .mpk
        FORMAT_CARTRIDGE              // .eep
    };

    public:
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

        FileLoader& getLoader() {
            return *loader;
        }

        int getControllerPakCurrentlySelectedSaveIndex() const {
            return controllerPakCurrentlySelectedSaveIndex;
        }

        void setControllerPakCurrentlySelectedSaveIndex(int controllerPakCurrentlySelectedSaveIndex_) {
            controllerPakCurrentlySelectedSaveIndex = controllerPakCurrentlySelectedSaveIndex_;
        }

        void openFile(const QString& filepath_);
        void writeFile(const QString& filepath_);

    private:
        static FileManager* instance;

        FileManager()  {
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
        void determineFormat();

        int format = FORMAT_NOTE;
        int controllerPakCurrentlySelectedSaveIndex = 0;    // The index of the currently selected save in a loaded Controller Pak

        QFile* file = nullptr;
        QByteArray* buffer = nullptr;
        QString filepath;
        FileLoader* loader = nullptr;
};

#endif
