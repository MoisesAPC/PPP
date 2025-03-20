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

        QString& getfilepath() {
            return filepath;
        }

        QFile& getFile() {
            return *file;
        }

        FileLoader& getLoader() {
            return *loader;
        }

        void openFile(const QString& filepath_);

    private:
        static FileManager* instance;

        FileManager()  {
            filepath = "";
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
        }

        FileManager(const FileManager& obj) = delete; // Remove the copy constructor
        void determineFormat();

        int format = FORMAT_NOTE;

        QFile* file = nullptr;
        QString filepath;
        FileLoader* loader = nullptr;
};

#endif
