#include "include/file/FileManager.h"
#include "include/save/SaveManager.h"

void FileManager::determineFormat() {
    if (!filepath.isEmpty()) {
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
        filepath = filepath_;

        determineFormat();
        file = new QFile(filepath);

        if (file->open(QIODevice::ReadOnly)) {
            getLoader().parseRegion(*file);
            getLoader().readAllSaveSlots(*file);
        }

        file->close();
    }
}
