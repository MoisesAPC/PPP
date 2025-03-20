#include "include/file/FileManager.h"
#include "include/save/SaveManager.h"

void FileManager::determineFormat(const QString& filename) {
    if (!filename.isEmpty()) {
        QFileInfo fileInfo(filename);

        QString fileExtension = fileInfo.suffix();

        if (fileExtension == "note") {
            loader = new FileLoaderNote();
        }
    }
}

void FileManager::openFile(const QString& filename) {
    if (!filename.isEmpty()) {
        determineFormat(filename);
        file = new QFile(filename);

        if (file->open(QIODevice::ReadOnly)) {
            getLoader().parseRegion(*file);
            getLoader().readAllSaveSlots(*file);
        }

        file->close();
    }
}
