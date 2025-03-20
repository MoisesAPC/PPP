#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFile>
#include <QtEndian>

class FileManager {
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

    private:
        static FileManager* instance;

        FileManager() {}
        ~FileManager() {}
        FileManager(const FileManager& obj) = delete; // Remove the copy constructor
};

#endif
