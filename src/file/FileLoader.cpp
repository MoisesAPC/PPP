/**
 * @file FileLoader.cpp
 * @brief FileLoader source code file
 *
 * This source code file contains the code for the file-handling operations.
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/file/FileLoader.h"
#include "include/file/FileManager.h"
#include <QDataStream>
#include <QDebug>
#include <algorithm> // std::search, std::distance
#include <cstddef>   // offsetof

/**
 * @brief Reads the data associated to a save slot from a file given the start offset within said file.
 */
void FileLoader::readSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset) {
    QDataStream inputStream(&file);

    // Return if we reached the end of the file
    if (startOffset >= getMaxFileSize()) {
        return;
    }

    slot.mainSave = readSaveData(inputStream, startOffset);
    slot.beginningOfStage = readSaveData(inputStream, inputStream.device()->pos());
    slot.checksum1 = readData<unsigned int>(inputStream, inputStream.device()->pos());
    slot.checksum2 = readData<unsigned int>(inputStream, inputStream.device()->pos());
}

/**
 * @brief Writes the data associated from a save slot to a file at the start offset within said file.
 */
void FileLoader::writeSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset) {
    SaveManager* saveManager = SaveManager::getInstance();
    QDataStream outputStream(&file);

    unsigned int firstChecksumOffset = 0;
    unsigned int secondChecksumOffset = 0;
    unsigned int saveDataSize = 0;

    if (SaveManager::getInstance()->getRegion() == SaveData::PAL) {
        firstChecksumOffset = offsetof(SaveSlot, checksum1);
        secondChecksumOffset = offsetof(SaveSlot, checksum2);
        saveDataSize = sizeof(SaveData);
    }
    else {
        // Since the PAL version of the SaveSlot added 4 extra bytes (and this project uses the PAL definition of the struct),
        // we will remove those extra 4 bytes (plus 4 -> 8 bytes) from the offset to get the correct address to write the checksums to.
        firstChecksumOffset = offsetof(SaveSlot, checksum1) - 8;
        secondChecksumOffset = offsetof(SaveSlot, checksum2) - 8;
        saveDataSize = sizeof(SaveData) - 4;
    }

    // Write the main and beginning of stage saves
    writeSaveData(outputStream, slot.mainSave, startOffset);
    writeSaveData(outputStream, slot.beginningOfStage, outputStream.device()->pos());

    /**
     * Write the checksums. To ensure the checksums are properly calculated,
     * we convert the data to a byte array, then swap the endianness (since the save file is stored in big endian)
     */
    QIODevice* device = outputStream.device();
    device->seek(startOffset);
    QByteArray rawData = device->read(saveDataSize);

    // Convert "rawData" to big-endian
    swapEndianness(&rawData);

    // Write the SaveSlot main's checksum at the specific offsets within the SaveSlot struct
    unsigned int firstChecksum = saveManager->calcFirstChecksum(rawData);
    unsigned int secondChecksum = saveManager->calcSecondChecksum(rawData);

    device->seek(startOffset + firstChecksumOffset);
    outputStream << firstChecksum;
    device->seek(startOffset + secondChecksumOffset);
    outputStream << secondChecksum;
}

/**
 * @brief Returns the region numeric ID given its equivalent character ID.
 */
short FileLoader::getRegionEnumFromChar(const unsigned char regionFromFile) {
    switch (regionFromFile) {
        default:
        case 'E':
            return SaveData::USA;

        case 'J':
            return SaveData::JPN;

        case 'P':
            return SaveData::PAL;
    }
}

/**
 * @brief Finds the region's character ID and sets the actual region value within the program accordingly.
 */
void FileLoaderNote::parseRegion(QFile& file) {
    SaveManager* saveManager = SaveManager::getInstance();
    QDataStream inputStream(&file);

    unsigned char regionFromFile = readData<unsigned char>(inputStream, getRegionIdOffset());
    saveManager->setRegion(getRegionEnumFromChar(regionFromFile));
}

/**
 * @brief Reads an entire save from the given file. The start offset for this data depends on the file format.
 */
void FileLoader::readAllSaveSlots(QFile& file) {
    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        readSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (getSaveSlotPaddedSize() * i));
    }
}

/**
 * @brief Writes an entire save to the given file. The start offset for this data depends on the file format.
 */
void FileLoader::writeAllSaveSlots(QFile& file) {
    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        writeSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (getSaveSlotPaddedSize() * i));
    }
}

/**
 * @brief Reads a save data entry from the given data stream. The start offset for this data depends on the file format.
 */
const SaveData& FileLoader::readSaveData(QDataStream& inputStream, unsigned int startOffset) {
    SaveData* currentSave = new SaveData();

    // Seek to the start of the save data
    inputStream.device()->seek(startOffset);

    // Read save data contents into "currentSave"
    for (unsigned int i = 0; i < NUM_EVENT_FLAGS; i++) {
        currentSave->event_flags[i] = readData<unsigned int>(inputStream, inputStream.device()->pos());
    }
    currentSave->flags = readData<unsigned int>(inputStream, inputStream.device()->pos());

    currentSave->week = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->day = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->hour = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->minute = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->seconds = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->milliseconds = readData<unsigned short>(inputStream, inputStream.device()->pos());
    currentSave->gameplay_framecount = readData<unsigned int>(inputStream, inputStream.device()->pos());

    currentSave->button_config = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->sound_mode = readData<short>(inputStream, inputStream.device()->pos());

    // PAL-exclusive data
    if (SaveManager::getInstance()->getRegion() == SaveData::PAL) {
        currentSave->language = readData<short>(inputStream, inputStream.device()->pos());
        currentSave->padding5A_PAL = readData<short>(inputStream, inputStream.device()->pos());
    }

    currentSave->character = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->life = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->field_0x5C = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->subweapon = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->gold = readData<unsigned int>(inputStream, inputStream.device()->pos());

    for (unsigned int j = 0; j < SIZE_ITEMS_ARRAY; j++) {
        currentSave->items[j] = readData<unsigned char>(inputStream, inputStream.device()->pos());
    }

    currentSave->player_status = readData<unsigned int>(inputStream, inputStream.device()->pos());
    currentSave->health_depletion_rate_while_poisoned = readData<short>(inputStream, inputStream.device()->pos());

    currentSave->current_hour_VAMP = readData<unsigned short>(inputStream, inputStream.device()->pos());
    currentSave->map = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->spawn = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->save_crystal_number = readData<unsigned short>(inputStream, inputStream.device()->pos());

    currentSave->field51_0xb2 = readData<unsigned char>(inputStream, inputStream.device()->pos());
    currentSave->field52_0xb3 = readData<unsigned char>(inputStream, inputStream.device()->pos());

    currentSave->time_saved_counter = readData<unsigned int>(inputStream, inputStream.device()->pos());
    currentSave->death_counter = readData<unsigned int>(inputStream, inputStream.device()->pos());

    currentSave->field55_0xbc = readData<int>(inputStream, inputStream.device()->pos());
    currentSave->field59_0xc0 = readData<int>(inputStream, inputStream.device()->pos());
    currentSave->field63_0xc4 = readData<int>(inputStream, inputStream.device()->pos());
    currentSave->field67_0xc8 = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->field69_0xca = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->field71_0xcc = readData<int>(inputStream, inputStream.device()->pos());
    currentSave->field75_0xd0 = readData<int>(inputStream, inputStream.device()->pos());
    currentSave->field77_0xd2 = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->field79_0xd4 = readData<short>(inputStream, inputStream.device()->pos());
    currentSave->field83_0xd8 = readData<int>(inputStream, inputStream.device()->pos());
    currentSave->gold_spent_on_Renon = readData<unsigned int>(inputStream, inputStream.device()->pos());

    return *currentSave;
}

/**
 * @brief Writes a save data entry to the given data stream. The start offset for this data depends on the file format.
 */
void FileLoader::writeSaveData(QDataStream& outputStream, const SaveData& saveData, unsigned int startOffset) {
    outputStream.device()->seek(startOffset);

    for (unsigned int i = 0; i < NUM_EVENT_FLAGS; i++) {
        writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.event_flags[i]);
    }

    writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.flags);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.week);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.day);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.hour);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.minute);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.seconds);
    writeData<unsigned short>(outputStream, outputStream.device()->pos(), saveData.milliseconds);
    writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.gameplay_framecount);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.button_config);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.sound_mode);

    // PAL-related saves
    if (SaveManager::getInstance()->getRegion() == SaveData::PAL) {
        writeData<short>(outputStream, outputStream.device()->pos(), saveData.language);
        writeData<short>(outputStream, outputStream.device()->pos(), saveData.padding5A_PAL);
    }

    writeData<short>(outputStream, outputStream.device()->pos(), saveData.character);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.life);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.field_0x5C);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.subweapon);
    writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.gold);

    for (unsigned int j = 0; j < SIZE_ITEMS_ARRAY; j++) {
        writeData<unsigned char>(outputStream, outputStream.device()->pos(), saveData.items[j]);
    }

    writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.player_status);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.health_depletion_rate_while_poisoned);
    writeData<unsigned short>(outputStream, outputStream.device()->pos(), saveData.current_hour_VAMP);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.map);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.spawn);
    writeData<unsigned short>(outputStream, outputStream.device()->pos(), saveData.save_crystal_number);
    writeData<unsigned char>(outputStream, outputStream.device()->pos(), saveData.field51_0xb2);
    writeData<unsigned char>(outputStream, outputStream.device()->pos(), saveData.field52_0xb3);
    writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.time_saved_counter);
    writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.death_counter);
    writeData<int>(outputStream, outputStream.device()->pos(), saveData.field55_0xbc);
    writeData<int>(outputStream, outputStream.device()->pos(), saveData.field59_0xc0);
    writeData<int>(outputStream, outputStream.device()->pos(), saveData.field63_0xc4);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.field67_0xc8);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.field69_0xca);
    writeData<int>(outputStream, outputStream.device()->pos(), saveData.field71_0xcc);
    writeData<int>(outputStream, outputStream.device()->pos(), saveData.field75_0xd0);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.field77_0xd2);
    writeData<short>(outputStream, outputStream.device()->pos(), saveData.field79_0xd4);
    writeData<int>(outputStream, outputStream.device()->pos(), saveData.field83_0xd8);
    writeData<unsigned int>(outputStream, outputStream.device()->pos(), saveData.gold_spent_on_Renon);
}

/**
 * @brief Get the raw header bytes for Note-formatted files.
 *
 * @note source: https://github.com/bryc/mpkedit/wiki/Note-file-formats
 * @note This implements the format last updated on Sep 29, 2023.
 */
std::vector<unsigned char> FileLoaderNote::getHeaderBytes() const {
    switch (SaveManager::getInstance()->getRegion()) {
        case SaveData::USA:
            return {
                0x01, 0x4D, 0x50, 0x4B, 0x4E, 0x6F, 0x74, 0x65, 0x00, 0x00, 0x00, 0x67,
                0x89, 0x7E, 0x56, 0x00, 0x4E, 0x44, 0x33, 0x45, 0x41, 0x34, 0xCA, 0xFE,
                0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1A, 0x2C, 0x2D,
                0x25, 0x1E, 0x2F, 0x1A, 0x27, 0x22, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00
            };

        case SaveData::JPN:
            return {
                0x01, 0x4D, 0x50, 0x4B, 0x4E, 0x6F, 0x74, 0x65, 0x00, 0x00, 0x00, 0x67,
                0x89, 0x7E, 0x56, 0x00, 0x4E, 0x44, 0x33, 0x4A, 0x41, 0x34, 0xCA, 0xFE,
                0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1A, 0x2C, 0x2D,
                0x25, 0x1E, 0x2F, 0x1A, 0x27, 0x22, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00
            };

        case SaveData::PAL:
            return {
                0x01, 0x4D, 0x50, 0x4B, 0x4E, 0x6F, 0x74, 0x65, 0x00, 0x00, 0x00, 0x67,
                0x89, 0x7E, 0x56, 0x00, 0x4E, 0x44, 0x33, 0x50, 0x41, 0x34, 0xCA, 0xFE,
                0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1A, 0x2C, 0x2D,
                0x25, 0x1E, 0x2F, 0x1A, 0x27, 0x22, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00
            };
    }

    return {};
}

/**
 * @note Cartridge saves are exclusive to the Japanese version.
 */
void FileLoaderCartridge::parseRegion(QFile& file) {
    SaveManager::getInstance()->setRegion(SaveData::JPN);
}

std::vector<unsigned char> FileLoaderCartridge::getHeaderBytes() const {
    return {
        0x4B, 0x43, 0x45, 0x4B, 0x20, 0x46, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x20, 0x31, 0x32, 0x30, 0x39
    };
}

/**
 * @brief Get the raw save slot size + its padding.
 */
unsigned int FileLoaderNote::getSaveSlotPaddedSize() const {
    return sizeof(SaveSlot) + (SaveSlot::getPaddedSize() - sizeof(SaveSlot));
}

/**
 * @note Always 0x900 bytes in practice.
 */
unsigned int FileLoaderNote::getMaxFileSize() const {
    unsigned int headerSize = getHeaderBytes().size();
    unsigned int maxFileSize = headerSize + (getSaveSlotPaddedSize() * NUM_SAVES) + getUnusedExtraSize();

    return maxFileSize;
};

bool FileLoader::searchHexInFile(const QByteArray& data, const std::vector<unsigned char>& target) {
    if (data.isEmpty() || target.empty()) {
        return false;
    }

    // Convert QByteArray to std::vector<unsigned char>
    std::vector<unsigned char> fileContents(data.begin(), data.end());

    // Search for the target sequence
    auto it = std::search(fileContents.begin(), fileContents.end(), target.begin(), target.end());

    return it != fileContents.end(); // True if found, false otherwise
}

unsigned int FileLoaderNote::countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const {
    if (data.isEmpty() || target.empty()) {
        return 0;
    }

    // Convert QByteArray to std::vector<unsigned char>
    std::vector<unsigned char> fileContents(data.begin(), data.end());

    int count = 0;
    auto it = fileContents.begin();

    // Search for all occurrences
    while (searchHexInFile(data, target)) {
        ++count;
        // Move past this occurrence
        it += getSaveSlotPaddedSize();

        // Break if the iterator reaches or exceeds the end of the buffer
        if (std::distance(fileContents.begin(), it) >= fileContents.size()) {
            break;
        }
    }

    return count;
}

unsigned int FileLoaderCartridge::countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const {
    if (data.isEmpty() || target.empty()) {
        return 0;
    }

    // Convert QByteArray to std::vector<unsigned char>
    std::vector<unsigned char> fileContents(data.begin(), data.end());

    int count = 0;
    auto it = fileContents.begin();

    // Search for all occurrences
    while (searchHexInFile(data, target)) {
        ++count;
        // Move past this occurrence
        it += getSaveSlotPaddedSize();

        // Break if the iterator reaches or exceeds the end of the buffer
        if (std::distance(fileContents.begin(), it) >= fileContents.size()) {
            break;
        }
    }

    return count;
}

unsigned int FileLoaderControllerPak::countHexOccurrences(const QByteArray& data, const std::vector<unsigned char>& target) const {
    if (data.isEmpty() || target.empty()) {
        return 0;
    }

    // Convert QByteArray to std::vector<unsigned char>
    std::vector<unsigned char> fileContents(data.begin(), data.end());

    int count = 0;
    auto it = fileContents.begin();

    // Search for all occurrences
    while (searchHexInFile(data, target)) {
        ++count;
        // Move past this occurrence
        it += getSaveSlotPaddedSize();

        // Break if the iterator reaches or exceeds the end of the buffer
        if (std::distance(fileContents.begin(), it) >= fileContents.size()) {
            break;
        }
    }

    return count;
}

/**
 * @brief Given a cartridge save (which has dynamic size), return the number of saves it currently has.
 */
unsigned int FileLoaderCartridge::getCartridgeNumSaves() const {
    // We search the number of times the cartridge header data has been found, which is equal to the number of saves the file has
    return countHexOccurrences(FileManager::getInstance()->getBuffer(), getHeaderBytes());
}

unsigned int FileLoaderCartridge::getSaveSlotPaddedSize() const {
    unsigned int headerSize = getHeaderBytes().size();

    // The complete save slot ends at offset 0x1F0, not 0x200, so we remove -0x10 bytes from it.
    // Besides, each slot now starts with the header data.
    return headerSize + (sizeof(SaveSlot) + (SaveSlot::getPaddedSize() - sizeof(SaveSlot)) - 0x10);
}

/**
 * @note Cartridge saves have variable size.
 */
unsigned int FileLoaderCartridge::getMaxFileSize() const {
    unsigned int maxFileSize = getSaveSlotPaddedSize() * getCartridgeNumSaves();

    return maxFileSize;
};

void FileLoaderControllerPak::parseRegion(QFile& file) {
    std::vector<FileManager::ControllerPakNotetableData>* noteTableArray = FileManager::getInstance()->getControllerPakNotetableDataArray();

    // Set the region of the currently selected Controller Pak save
    SaveManager::getInstance()->setRegion((*noteTableArray)[FileManager::getInstance()->getControllerPakCurrentlySelectedSaveIndex()].region);
}

unsigned int FileLoaderControllerPak::getRawDataOffsetStart() const {
    // We obtain the raw data offset start from the note table data.
    std::vector<FileManager::ControllerPakNotetableData>* noteTableArray = FileManager::getInstance()->getControllerPakNotetableDataArray();
    return (*noteTableArray)[FileManager::getInstance()->getControllerPakCurrentlySelectedSaveIndex()].rawDataStartOffset;
}

unsigned int FileLoaderControllerPak::getSaveSlotPaddedSize() const {
    return sizeof(SaveSlot) + (SaveSlot::getPaddedSize() - sizeof(SaveSlot));
}

/**
 * @note Always 0x8000 bytes
 */
unsigned int FileLoaderControllerPak::getMaxFileSize() const {
    return 0x8000;
};

/**
 * @note In DexDrive saves, the Controller Pak data actually starts at 0x1040.
 */
unsigned int FileLoaderDexDrive::getMaxFileSize() const {
    return 0x8000 + 0x1040;
};

/**
 * @brief Given an byte array, it swaps the endianness between little endian<->big endian
 */
void FileLoader::swapEndianness(QByteArray* rawData) {
    if (!rawData || rawData->size() < 4) {
        return;
    }

    char* data = rawData->data();
    int dataSize = rawData->size();

    for (int i = 0; i + 3 < dataSize; i += 4) {
        std::swap(data[i], data[i + 3]);
        std::swap(data[i + 1], data[i + 2]);
    }
}

void FileLoaderCartridge::writeAllSaveSlots(QFile& file) {
    file.seek(0);

    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        // First, write the header, then the saveslot data
        std::vector<unsigned char> headerBytes = getHeaderBytes();

        if (!headerBytes.empty()) {
            file.write(reinterpret_cast<const char*>(headerBytes.data()), headerBytes.size());
        }

        writeSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (getSaveSlotPaddedSize() * i));

        // Add padding bytes at the end of each saveslot
        std::vector<unsigned char> paddingBytes(getSaveSlotPaddingBytesSize(), 0);
        file.write(reinterpret_cast<const char*>(paddingBytes.data()), paddingBytes.size());
    }
}

void FileLoaderNote::writeAllSaveSlots(QFile& file) {
    file.seek(0);

    // First, write the header, then the saveslot data
    std::vector<unsigned char> headerBytes = getHeaderBytes();
    if (!headerBytes.empty()) {
        file.write(reinterpret_cast<const char*>(headerBytes.data()), headerBytes.size());
    }

    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        writeSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (getSaveSlotPaddedSize() * i));

        // Add padding bytes at the end of each saveslot
        std::vector<unsigned char> paddingSlotBytes(getSaveSlotPaddingBytesSize(), 0);
        file.write(reinterpret_cast<const char*>(paddingSlotBytes.data()), paddingSlotBytes.size());
    }

    // Add padding bytes at the end of the whole file
    std::vector<unsigned char> paddingBytes(getUnusedExtraSize(), 0);
    file.write(reinterpret_cast<const char*>(paddingBytes.data()), paddingBytes.size());
}

/**
 * @brief Get the size of the padding data after the end of the actual save slot data.
 */
unsigned int FileLoaderNote::getSaveSlotPaddingBytesSize() const {
    unsigned int paddingBytes = SaveSlot::getPaddedSize() - sizeof(SaveSlot);

    switch (SaveManager::getInstance()->getRegion()) {
        case SaveData::USA:
        case SaveData::JPN:
        default:
            // Add the extra 8 bytes found in the PAL version of the saveslot
            return paddingBytes + 8;

        case SaveData::PAL:
            return paddingBytes;
    }
}

/**
 * @brief Get the size of the padding data after the end of the actual save slot data.
 */
unsigned int FileLoaderCartridge::getSaveSlotPaddingBytesSize() const {
    unsigned int paddingBytes = SaveSlot::getPaddedSize() - sizeof(SaveSlot) - getHeaderBytes().size();

    switch (SaveManager::getInstance()->getRegion()) {
    case SaveData::USA:
    case SaveData::JPN:
    default:
        // Add the extra 8 bytes found in the PAL version of the saveslot
        return paddingBytes + 8;

    case SaveData::PAL:
        return paddingBytes;
    }
}

int FileLoaderNote::checkFileOpenErrors() {
    FileManager* fileManager = FileManager::getInstance();

    // Ensure the file has the predefined size (0x900 bytes in practice)
    if (fileManager->getFile().size() != getMaxFileSize()) {
        return -1;
    }

    return 0;
}

int FileLoaderCartridge::checkFileOpenErrors() {
    FileManager* fileManager = FileManager::getInstance();

    // Ensure the file has the predefined size (0x900 bytes in practice),
    // + that there's at least one valid save on it.
    if (fileManager->getFile().size() != getMaxFileSize() ||
        getCartridgeNumSaves() == 0) {
        return -1;
    }

    return 0;
}

int FileLoaderControllerPak::checkFileOpenErrors() {
    FileManager* fileManager = FileManager::getInstance();

    // Ensure the file has the predefined size (0x900 bytes in practice)
    if (fileManager->getFile().size() != getMaxFileSize()) {
        return -1;
    }

    return 0;
}
