#ifndef FILELOADER_H
#define FILELOADER_H

/******************************************************************************
 * FileLoader.h
 *
 * Header file for FileLoader.cpp
 *****************************************************************************/

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
    virtual void writeAllSaveSlots(QFile& file);
    virtual void parseRegion(QFile& file) = 0;
    const SaveData& parseSaveData(QDataStream& inputStream, unsigned int startOffset);
    void writeSaveData(QDataStream& outputStream, const SaveData& saveData, unsigned int startOffset);
    static bool searchHexInFile(const QByteArray& data, const std::vector<unsigned char>& target);
    virtual unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const = 0;
    virtual unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; };
    virtual unsigned int getRegionIdOffset() const { return regionIdOffset; };
    virtual unsigned int getMaxFileSize() const { return 0; }
    virtual unsigned int getUnusedExtraSize() const { return 0; }
    virtual unsigned int getSaveSlotPaddedSize() const = 0;
    virtual unsigned int getSaveSlotPaddingBytesSize() const { return 0; }
    virtual std::vector<unsigned char> getHeaderBytes() const = 0;
    short getRegionEnumFromChar(const unsigned char regionFromFile);
    virtual unsigned int getNoteTableOffset() const { return 0; }
    virtual unsigned int getNoteTableEntrySize() const { return 0; }
    virtual unsigned int getNoteTableNumEntries() const { return 0; }
    virtual unsigned int getRawDataOffsetPerEntry(unsigned int rawDataStartOffsetByte) const { return 0; };
    void swapEndianness(QByteArray*);

    template<typename T>
    T readData(QDataStream& inputStream, long offset) {
        inputStream.device()->seek(offset);

        T value;
        inputStream.readRawData(reinterpret_cast<char*>(&value), sizeof(T));

        return qFromBigEndian(value);
    }

    template<typename T>
    void writeData(QDataStream& outputStream, long offset, T value) {
        outputStream.device()->seek(offset);

        T bigEndianValue = qToBigEndian(value);
        outputStream.writeRawData(reinterpret_cast<char*>(&bigEndianValue), sizeof(T));
    }
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

    void writeAllSaveSlots(QFile& file);
    unsigned int getSaveSlotPaddingBytesSize() const;
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

    void writeAllSaveSlots(QFile& file);
    unsigned int getSaveSlotPaddingBytesSize() const;
};

struct FileLoaderControllerPak: public FileLoader {
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_OFFSET = 0x300;
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE = 0x20;
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES = 16;

    FileLoaderControllerPak() {}
    ~FileLoaderControllerPak() {}
    void parseRegion(QFile& file);
    unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const;
    unsigned int getRawDataOffsetStart() const;
    unsigned int getRegionIdOffset() const { return 0; }
    unsigned int getMaxFileSize() const;
    unsigned int getUnusedExtraSize() const { return 0x100; };  // Unused extra 100 bytes at the end of notes
    unsigned int getSaveSlotPaddedSize() const;
    std::vector<unsigned char> getHeaderBytes() const { return {}; }

    unsigned int getNoteTableOffset() const { return CONTROLLER_PAK_NOTE_TABLE_OFFSET; }
    unsigned int getNoteTableEntrySize() const { return CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE; }
    unsigned int getNoteTableNumEntries() const { return CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES; }
    unsigned int getRawDataOffsetPerEntry(unsigned int rawDataStartOffsetByte) const { return rawDataStartOffsetByte * 0x100; };
};

struct FileLoaderDexDrive: public FileLoaderControllerPak {
    // In DexDrive saves, the Controller Pak data actually starts at 0x1040
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_OFFSET = 0x300 + 0x1040;
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE = 0x20;
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES = 16;

    FileLoaderDexDrive() {}
    ~FileLoaderDexDrive() {}

    unsigned int getMaxFileSize() const;
    unsigned int getNoteTableOffset() const { return CONTROLLER_PAK_NOTE_TABLE_OFFSET; }
    unsigned int getNoteTableEntrySize() const { return CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE; }
    unsigned int getNoteTableNumEntries() const { return CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES; }
    unsigned int getRawDataOffsetPerEntry(unsigned int rawDataStartOffsetByte) const { return (rawDataStartOffsetByte * 0x100) + 0x1040; };
};

#endif
