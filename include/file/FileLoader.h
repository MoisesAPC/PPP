#ifndef FILELOADER_H
#define FILELOADER_H

#include "include/save/SaveManager.h"

struct FileLoader {
    const unsigned int rawDataStartOffset = 0;
    const unsigned int regionIdOffset = 0;

    FileLoader() {}
    virtual ~FileLoader() {}

    void readSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset);
    void readAllSaveSlots(QFile& file);
    void writeSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset);
    void writeAllSaveSlots(QFile& file);
    virtual void parseRegion(QFile& file) {}
    const SaveData& parseSaveData(QDataStream& inputStream, unsigned int startOffset);
    void writeSaveData(QDataStream& outputStream, const SaveData& saveData, unsigned int startOffset);
    virtual unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; };
    virtual unsigned int getRegionIdOffset() const { return regionIdOffset; };

    template<typename T>
    T readData(QDataStream& inputStream, long offset);
};

struct FileLoaderNote: public FileLoader {
    const unsigned int rawDataStartOffset = 0x30;
    const unsigned int regionIdOffset = 0x13;

    FileLoaderNote() {}
    ~FileLoaderNote() {}
    void parseRegion(QFile& file);
    unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; };
    unsigned int getRegionIdOffset() const { return regionIdOffset; };
};

#endif
