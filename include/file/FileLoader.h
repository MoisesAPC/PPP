#ifndef FILELOADER_H
#define FILELOADER_H

/**
 * @file FileLoader.h
 * @brief FileLoader header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/save/SaveManager.h"
#include <vector>

/**
 * @class FileLoader
 * @brief FileLoader abstract class
 *
 * This abstract class contains the default implementation for the FileLoader data structure,
 * which is in charge of all file-related operations (opening files, closing them, parsing them, etc).
 */
class FileLoader {
    /// @note All of these offsets are relative to the very beginning of the file.
    const unsigned int rawDataStartOffset = 0;  /**< Offset where the binary SaveData starts within the file */
    const unsigned int regionIdOffset = 0;      /**< Offset where region identification is stored at */
    const unsigned int maxFileSize = 0;         /**< Maximum allowed size for the file format we're currently handling */

    public:
        // Constructors and destructor
        FileLoader() {}
        virtual ~FileLoader() {}

        // Main file read and write functions
        virtual void parseRegion(QFile& file) = 0;
        void readAllSaveSlots(QFile& file);
        virtual void writeAllSaveSlots(QFile& file);
        void readSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset);
        void writeSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset);
        const SaveData& readSaveData(QDataStream& inputStream, unsigned int startOffset);
        void writeSaveData(QDataStream& outputStream, const SaveData& saveData, unsigned int startOffset);

        // Search-related functions
        // Search occurences of an array of bytes in a QByteArray, and count its occurences, respectively.
        static bool searchHexInFile(const QByteArray& data, const std::vector<unsigned char>& target);
        virtual unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const = 0;

        // Getter functions related to file-handling tasks
        virtual unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; }
        virtual unsigned int getRegionIdOffset() const { return regionIdOffset; }
        virtual unsigned int getMaxFileSize() const { return 0; }
        virtual unsigned int getUnusedExtraSize() const { return 0; }
        virtual unsigned int getSaveSlotPaddedSize() const = 0;
        virtual unsigned int getSaveSlotPaddingBytesSize() const { return 0; }
        virtual unsigned int getSavePaddedSize() const { return 0x200; }

        /**
         * @brief getHeaderBytes
         * Get the raw bytes associated to the file's header, when applicable.
         */
        virtual std::vector<unsigned char> getHeaderBytes() const = 0;
        virtual unsigned int getNoteTableOffset() const { return 0; }
        virtual unsigned int getNoteTableEntrySize() const { return 0; }
        virtual unsigned int getNoteTableNumEntries() const { return 0; }

        /**
         * @brief getRawDataOffsetPerEntry
         *
         * Given a single raw data offset byte, it calculates the actual offset where the save
         * can be found relative to the start of the whole binary file
         *
         * For example, for Controller Paks, if the raw data byte is associated to a given save is 0x05,
         * then the actual offset where the raw data for that save starts is at 0x05 * 0x100 = 0x500
         */
        virtual unsigned int getRawDataOffsetPerEntry(unsigned int rawDataStartOffsetByte) const { return 0; }

        void swapEndianness(QByteArray*);

        short getRegionEnumFromChar(const unsigned char regionFromFile);

        /**
         * Reads a value of type T at the given offset within the input stream's raw data.
         */
        template<typename T>
        T readData(QDataStream& inputStream, long offset) {
            inputStream.device()->seek(offset);

            T value;
            inputStream.readRawData(reinterpret_cast<char*>(&value), sizeof(T));

            return qFromBigEndian(value);
        }

        /**
         * Writes a value of type T at the given offset within the input stream's raw data.
         */
        template<typename T>
        void writeData(QDataStream& outputStream, long offset, T value) {
            outputStream.device()->seek(offset);

            T bigEndianValue = qToBigEndian(value);
            outputStream.writeRawData(reinterpret_cast<char*>(&bigEndianValue), sizeof(T));
        }

        /**
         * Error verification when opening a file.
         *
         * @return -1 on fail. 0 on success.
         */
        virtual int checkFileOpenErrors() = 0;
};

/**
 * @class FileLoaderNote
 * @brief FileLoaderNote class
 *
 * This class handles reading and writing .note files (individual notes from a Controller Pak)
 */
class FileLoaderNote: public FileLoader {
    /// @note The raw data starts right after the header,
    /// hence why we just get the size of the header bytes
    const unsigned int rawDataStartOffset = getHeaderBytes().size();
    const unsigned int regionIdOffset = 0x13;

    public:
        // Constructors and destructor
        FileLoaderNote() {}
        ~FileLoaderNote() {}

        // Main file read and write functions
        void parseRegion(QFile& file);
        void writeAllSaveSlots(QFile& file);

        // Getter functions related to file-handling tasks
        unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const;
        unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; }
        unsigned int getRegionIdOffset() const { return regionIdOffset; }
        unsigned int getMaxFileSize() const;
        unsigned int getUnusedExtraSize() const { return 0x100; }  // Unused extra 100 bytes at the end of notes
        unsigned int getSaveSlotPaddedSize() const;
        std::vector<unsigned char> getHeaderBytes() const;
        unsigned int getSaveSlotPaddingBytesSize() const;
        int checkFileOpenErrors();
};

/**
 * @class FileLoaderCartridge
 * @brief FileLoaderCartridge class
 *
 * This class handles reading and writing .eep files
 * (save files embedded in the cartridge, only found in the Japanese version of the game)
 */
class FileLoaderCartridge: public FileLoader {
    /// @note The raw data starts right after the header,
    /// hence why we just get the size of the header bytes
    const unsigned int rawDataStartOffset = getHeaderBytes().size();
    const unsigned int regionIdOffset = 0;  /// @note Not needed for cartridge saves

    public:
        // Constructors and destructor
        FileLoaderCartridge() {}
        ~FileLoaderCartridge() {}

        // Main file read and write functions
        void parseRegion(QFile& file);
        void writeAllSaveSlots(QFile& file);

        // Getter functions related to file-handling tasks
        unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const;
        unsigned int getRawDataOffsetStart() const { return rawDataStartOffset; }
        unsigned int getRegionIdOffset() const { return 0; }   // Not needed for cartridge saves, so we return 0
        unsigned int getMaxFileSize() const;
        unsigned int getUnusedExtraSize() const { return 0; }
        unsigned int getSaveSlotPaddedSize() const;
        std::vector<unsigned char> getHeaderBytes() const;
        unsigned int getCartridgeNumSaves() const;
        unsigned int getSaveSlotPaddingBytesSize() const;
        int checkFileOpenErrors();
};

/**
 * @class FileLoaderControllerPak
 * @brief FileLoaderControllerPak class
 *
 * This class handles reading and writing .mpk / .pak files (Controller Pak containers).
 *
 * The "note table" referenced below is a table that contains metadata for each save, including
 * the start offset of the raw data associated to each save, its region, etc.
 */
struct FileLoaderControllerPak: public FileLoader {
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_OFFSET = 0x300;    /**< Offset where the note table starts */
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE = 0x20; /**< Size of each entry in the note table */
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES = 16;  /**< Total number of elements in the note table */

    public:
        // Constructors and destructor
        FileLoaderControllerPak() {}
        ~FileLoaderControllerPak() {}

        // Main file read and write functions
        void parseRegion(QFile& file);

        // Getter functions related to file-handling tasks
        unsigned int countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const;
        unsigned int getRawDataOffsetStart() const;
        unsigned int getRegionIdOffset() const { return 0; }
        unsigned int getMaxFileSize() const;
        unsigned int getUnusedExtraSize() const { return 0x100; }  // Unused extra 100 bytes at the end of notes
        unsigned int getSaveSlotPaddedSize() const;
        std::vector<unsigned char> getHeaderBytes() const { return {}; }
        unsigned int getNoteTableOffset() const { return CONTROLLER_PAK_NOTE_TABLE_OFFSET; }
        unsigned int getNoteTableEntrySize() const { return CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE; }
        unsigned int getNoteTableNumEntries() const { return CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES; }
        unsigned int getRawDataOffsetPerEntry(unsigned int rawDataStartOffsetByte) const { return rawDataStartOffsetByte * 0x100; }
        int checkFileOpenErrors();
};

/**
 * @class FileLoaderDexDrive
 * @brief FileLoaderDexDrive class
 *
 * This class handles reading and writing .t64 / .n64 files
 * (Controller Pak containers in the DexDrive format. This is why it inherits from FileLoaderControllerPak)
 *
 * @note In DexDrive saves, the Controller Pak data actually starts at 0x1040, which is why
 * there are various 0x1040 constants in the code for this class.
 */
struct FileLoaderDexDrive: public FileLoaderControllerPak {
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_OFFSET = 0x300 + 0x1040;
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE = 0x20;
    const unsigned int CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES = 16;

    public:
        // Constructors and destructor
        FileLoaderDexDrive() {}
        ~FileLoaderDexDrive() {}

        // Getter functions related to file-handling tasks
        unsigned int getMaxFileSize() const;
        unsigned int getNoteTableOffset() const { return CONTROLLER_PAK_NOTE_TABLE_OFFSET; }
        unsigned int getNoteTableEntrySize() const { return CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE; }
        unsigned int getNoteTableNumEntries() const { return CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES; }
        unsigned int getRawDataOffsetPerEntry(unsigned int rawDataStartOffsetByte) const { return (rawDataStartOffsetByte * 0x100) + 0x1040; }
};

#endif
