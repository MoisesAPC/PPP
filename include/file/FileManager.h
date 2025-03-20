#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "include/file/FileLoader.h"
#include <QFile>
#include <QtEndian>
#include <QFileInfo>

class FileManager {

    // The file format of the opened file
    enum eFormat {
        FORMAT_NOTE,        // .note
        FORMAT_PAK,         // .pak, .mpak
        FORMAT_CARTRIDGE    // .eep
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

        QString& getFilename() {
            return *filename;
        }

        QFile& getFile() {
            return *file;
        }

        FileLoader& getLoader() {
            return *loader;
        }

        void openFile(const QString& filename);

    private:
        static FileManager* instance;

        FileManager() {
            filename = new QString("");
        }

        ~FileManager() {
            if (filename != nullptr) {
                delete filename;
                filename = nullptr;
            }

            if (file != nullptr) {
                file->close();

                delete file;
                file = nullptr;
            }

            if (loader != nullptr) {
                delete loader;
                loader = nullptr;
            }
        }

        FileManager(const FileManager& obj) = delete; // Remove the copy constructor
        void determineFormat(const QString& filename);

        int format = FORMAT_NOTE;

        QFile* file = nullptr;
        QString* filename = nullptr;
        FileLoader* loader = nullptr;
};

#endif
