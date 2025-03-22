#include "include/file/FileLoader.h"
#include "include/file/FileManager.h"
#include <QDataStream>
#include <QDebug>
#include <algorithm> // std::search, std::distance

void FileLoader::readSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset) {
    QDataStream inputStream(&file);

    // Return if we reached the end of the file
    if (startOffset >= getMaxFileSize()) {
        return;
    }

    slot.main = parseSaveData(inputStream, startOffset);
    slot.beginningOfStage = parseSaveData(inputStream, inputStream.device()->pos());
    slot.checksum1 = readData<unsigned int>(inputStream, inputStream.device()->pos());
    slot.checksum2 = readData<unsigned int>(inputStream, inputStream.device()->pos());
}

void FileLoader::writeSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset) {
    SaveManager* saveManager = SaveManager::getInstance();
    QDataStream outputStream(&file);

    writeSaveData(outputStream, slot.main, startOffset);
    writeSaveData(outputStream, slot.beginningOfStage, outputStream.device()->pos());
    unsigned int firstChecksum = saveManager->calcFirstChecksum(reinterpret_cast<unsigned char*>(&slot.main));
    unsigned int secondChecksum = saveManager->calcSecondChecksum(reinterpret_cast<unsigned int*>(&slot.main));
    outputStream << firstChecksum;
    outputStream << secondChecksum;
}

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

void FileLoaderNote::parseRegion(QFile& file) {
    SaveManager* saveManager = SaveManager::getInstance();
    QDataStream inputStream(&file);

    unsigned char regionFromFile = readData<unsigned char>(inputStream, getRegionIdOffset());
    saveManager->setRegion(getRegionEnumFromChar(regionFromFile));
}

void FileLoader::readAllSaveSlots(QFile& file) {
    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        readSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (getSaveSlotPaddedSize() * i));
    }
}

void FileLoader::writeAllSaveSlots(QFile& file) {
    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        writeSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (getSaveSlotPaddedSize() * i));
    }
}

const SaveData& FileLoader::parseSaveData(QDataStream& inputStream, unsigned int startOffset) {
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

void FileLoader::writeSaveData(QDataStream& outputStream, const SaveData& saveData, unsigned int startOffset) {
    outputStream.device()->seek(startOffset);

    for (unsigned int i = 0; i < NUM_EVENT_FLAGS; i++) {
        outputStream << saveData.event_flags[i];
    }

    outputStream << saveData.flags
                 << saveData.week
                 << saveData.day
                 << saveData.hour
                 << saveData.minute
                 << saveData.seconds
                 << saveData.milliseconds
                 << saveData.gameplay_framecount
                 << saveData.button_config
                 << saveData.sound_mode
                 << saveData.character
                 << saveData.life
                 << saveData.field_0x5C
                 << saveData.subweapon
                 << saveData.gold;

    for (unsigned int j = 0; j < SIZE_ITEMS_ARRAY; j++) {
        outputStream << saveData.items[j];
    }

    outputStream << saveData.player_status
                 << saveData.health_depletion_rate_while_poisoned
                 << saveData.current_hour_VAMP
                 << saveData.map
                 << saveData.spawn
                 << saveData.save_crystal_number
                 << saveData.field51_0xb2
                 << saveData.field52_0xb3
                 << saveData.time_saved_counter
                 << saveData.death_counter
                 << saveData.field55_0xbc
                 << saveData.field59_0xc0
                 << saveData.field63_0xc4
                 << saveData.field67_0xc8
                 << saveData.field69_0xca
                 << saveData.field71_0xcc
                 << saveData.field75_0xd0
                 << saveData.field77_0xd2
                 << saveData.field79_0xd4
                 << saveData.field83_0xd8
                 << saveData.gold_spent_on_Renon;
}

template<typename T>
T FileLoader::readData(QDataStream& inputStream, long offset) {
    inputStream.device()->seek(offset);

    T value;
    inputStream.readRawData(reinterpret_cast<char*>(&value), sizeof(T));

    return qFromBigEndian(value);
}

// Header format information (https://github.com/bryc/mpkedit/wiki/Note-file-formats)
// This implements the format last updated on Sep 29, 2023
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

// Cartridge saves are exclusive to the JPN version
void FileLoaderCartridge::parseRegion(QFile& file) {
    SaveManager::getInstance()->setRegion(SaveData::JPN);
}

std::vector<unsigned char> FileLoaderCartridge::getHeaderBytes() const {
    return {
        0x4B, 0x43, 0x45, 0x4B, 0x20, 0x46, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x20, 0x31, 0x32, 0x30, 0x39
    };
}

unsigned int FileLoaderNote::getSaveSlotPaddedSize() const {
    return sizeof(SaveSlot) + (SaveSlot::getPaddedSize() - sizeof(SaveSlot));
}

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
        it += getSaveSlotPaddedSize(); // Move past this occurrence

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
        it += getSaveSlotPaddedSize(); // Move past this occurrence

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
        it += getSaveSlotPaddedSize(); // Move past this occurrence

        // Break if the iterator reaches or exceeds the end of the buffer
        if (std::distance(fileContents.begin(), it) >= fileContents.size()) {
            break;
        }
    }

    return count;
}

// Given a cartridge save (which has dynamic size), return the number of saves it currently has
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

// Cartridge saves have variable size
unsigned int FileLoaderCartridge::getMaxFileSize() const {
    unsigned int maxFileSize = getSaveSlotPaddedSize() * getCartridgeNumSaves();

    return maxFileSize;
};

// Initialize the FileLoaderControllerPak's "indexDataArray", in order to know extra information regarding each Castlevania save it has
unsigned int FileLoaderControllerPak::initIndexData(QFile& file) {
    SaveManager* saveManager = SaveManager::getInstance();
    unsigned int numCV64Saves = 0;
    QDataStream inputStream(&file);

    for (int i = 0; i < CONTROLLER_PAK_NOTE_TABLE_NUM_ENTRIES; i++) {
        const unsigned int GAMEID_SIZE = 6;
        QByteArray gameId(GAMEID_SIZE, '\0');

        inputStream.device()->seek(CONTROLLER_PAK_NOTE_TABLE_OFFSET + (CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE * i));   // This is where the current entry data starts
        unsigned int bytesRead = inputStream.readRawData(gameId.data(), GAMEID_SIZE);
        inputStream.device()->seek(CONTROLLER_PAK_NOTE_TABLE_OFFSET + (CONTROLLER_PAK_NOTE_TABLE_ENTRY_SIZE * i));   // Go back to where we were previously to reading gameId

        if (bytesRead != GAMEID_SIZE) {
            return 0;
        }

        // If the game save stored doesn't belong to any of the Castlevania 64 versions, skip to the next save
        if (gameId != "ND3EA4" && gameId != "ND3PA4" && gameId != "ND3JA4") {
            continue;
        }

        // Get the save index within the Controller Pak data
        indexDataArray[i].index = i;

        // Parse the region (at offset +3)
        unsigned char regionFromFile = readData<unsigned char>(inputStream, inputStream.device()->pos() + 3);
        indexDataArray[i].region = getRegionEnumFromChar(regionFromFile);

        // Parse the raw data start offset (at offset +3 after the region, and then multiplied by 0x100)
        unsigned int rawDataStartOffsetByte = readData<unsigned char>(inputStream, inputStream.device()->pos() + 3);
        indexDataArray[i].rawDataStartOffset = rawDataStartOffsetByte * 0x100;

        ++numCV64Saves;
    }

    return numCV64Saves;
}

void FileLoaderControllerPak::parseRegion(QFile& file) {
    // Set the region of the currently selected Controller Pak save
    SaveManager::getInstance()->setRegion(indexDataArray[FileManager::getInstance()->getControllerPakCurrentlySelectedSaveIndex()].region);
}

unsigned int FileLoaderControllerPak::getRawDataOffsetStart() const {
    return indexDataArray[FileManager::getInstance()->getControllerPakCurrentlySelectedSaveIndex()].rawDataStartOffset;
}

unsigned int FileLoaderControllerPak::getSaveSlotPaddedSize() const {
    return sizeof(SaveSlot) + (SaveSlot::getPaddedSize() - sizeof(SaveSlot));
}

unsigned int FileLoaderControllerPak::getMaxFileSize() const {
    unsigned int headerSize = getHeaderBytes().size();
    unsigned int maxFileSize = headerSize + (getSaveSlotPaddedSize() * NUM_SAVES) + getUnusedExtraSize();

    return maxFileSize;
};
