#ifndef FILELOADER_H
#define FILELOADER_H

#include "include/save/SaveManager.h"
#include <vector>

struct FileLoader {
    const unsigned int rawDataStartOffset = 0;
    const unsigned int regionIdOffset = 0;
    const unsigned int maxFileSize = 0;

    FileLoader() {}
    virtual ~FileLoader() {}

    void readSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset);
    void readAllSaveSlots(QFile& file);
    void writeSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset);
    void writeAllSaveSlots(QFile& file);
    virtual void parseRegion(QFile& file) = 0;
    const SaveData& parseSaveData(QDataStream& inputStream, unsigned int startOffset);
    void writeSaveData(QDataStream& outputStream, const SaveData& saveData, unsigned int startOffset);
    static bool searchHexInFile(const QByteArray& data, const std::vector<unsigned char>& target);
    virtual unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const = 0;
    virtual unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; };
    virtual unsigned int getRegionIdOffset() const { return regionIdOffset; };
    virtual unsigned int getMaxFileSize() const { return 0; };
    virtual unsigned int getUnusedExtraSize() const { return 0; };
    virtual unsigned int getSaveSlotPaddedSize() const = 0;
    virtual std::vector<unsigned char> getHeaderBytes() const = 0;

    template<typename T>
    T readData(QDataStream& inputStream, long offset);
};

struct FileLoaderNote: public FileLoader {
    const unsigned int rawDataStartOffset = getHeaderBytes().size();    // The raw data starts right after the header
    const unsigned int regionIdOffset = 0x13;

    FileLoaderNote() {}
    ~FileLoaderNote() {}
    void parseRegion(QFile& file);
    unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const;
    unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; };
    unsigned int getRegionIdOffset() const { return regionIdOffset; };
    unsigned int getMaxFileSize() const;
    unsigned int getUnusedExtraSize() const { return 0x100; };  // Unused extra 100 bytes at the end of notes
    unsigned int getSaveSlotPaddedSize() const;
    std::vector<unsigned char> getHeaderBytes() const;
};

struct FileLoaderCartridge: public FileLoader {
    const unsigned int rawDataStartOffset = getHeaderBytes().size();    // The raw data starts right after the header
    const unsigned int regionIdOffset = 0;  // Not needed for cartridge saves

    FileLoaderCartridge() {}
    ~FileLoaderCartridge() {}
    void parseRegion(QFile& file);
    unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const;
    unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; };
    unsigned int getRegionIdOffset() const { return 0; };   // Not needed for cartridge saves, so we return 0
    unsigned int getMaxFileSize() const;
    unsigned int getUnusedExtraSize() const { return 0; };
    unsigned int getSaveSlotPaddedSize() const;
    std::vector<unsigned char> getHeaderBytes() const;
    unsigned int getCartridgeNumSaves() const;
};

#endif
