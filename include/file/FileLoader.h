#ifndef FILELOADER_H
#define FILELOADER_H

#include "include/save/SaveManager.h"

struct FileLoader {
    const unsigned int rawDataStartOffset = 0;
    const unsigned int regionIdOffset = 0;

    FileLoader() {}
    virtual ~FileLoader() {}

    void readSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset);
    virtual void readAllSaveSlots(QFile& file) = 0;
    virtual void parseRegion(QFile& file) {}
    const SaveData& parseSaveData(QDataStream& inputStream, unsigned int startOffset);

    template<typename T>
    T readData(QDataStream& inputStream, long offset);
    template<typename T>
    void writeData(QDataStream& outputStream, const long offset, T value);
};

struct FileLoaderNote: public FileLoader {
    const unsigned int rawDataStartOffset = 0x30;
    const unsigned int regionIdOffset = 0x13;

    FileLoaderNote() {}
    ~FileLoaderNote() {}
    void readAllSaveSlots(QFile& file);
    void parseRegion(QFile& file);
};

#endif
