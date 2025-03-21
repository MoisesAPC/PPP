#include "include/file/FileLoader.h"
#include <QDataStream>

void FileLoader::readSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset) {
    QDataStream inputStream(&file);

    slot.main = parseSaveData(inputStream, startOffset);
    slot.beginningOfStage = parseSaveData(inputStream, inputStream.device()->pos());
    slot.checksum1 = readData<unsigned int>(inputStream, inputStream.device()->pos());
    slot.checksum2 = readData<unsigned int>(inputStream, inputStream.device()->pos());
}

void FileLoader::writeSaveSlot(QFile& file, SaveSlot& slot, unsigned int startOffset) {
    SaveManager* saveManager = SaveManager::getInstance();
    QDataStream outputStream(&file);
    outputStream.setByteOrder(QDataStream::BigEndian);

    writeSaveData(outputStream, slot.main, startOffset);
    writeSaveData(outputStream, slot.beginningOfStage, outputStream.device()->pos());

    unsigned int firstChecksum = saveManager->calcFirstChecksum(reinterpret_cast<unsigned char*>(&slot.main));
    unsigned int secondChecksum = saveManager->calcSecondChecksum(reinterpret_cast<unsigned int*>(&slot.main));
    outputStream << firstChecksum;
    outputStream << secondChecksum;
}

void FileLoaderNote::parseRegion(QFile& file) {
    SaveManager* saveManager = SaveManager::getInstance();
    QDataStream inputStream(&file);

    unsigned char regionFromFile = readData<unsigned char>(inputStream, 0x13);

    switch (regionFromFile) {
    case 'E':
        saveManager->setRegion(SaveData::USA);
        break;

    case 'J':
        saveManager->setRegion(SaveData::JPN);
        break;

    case 'P':
        saveManager->setRegion(SaveData::PAL);
        break;
    }
}

void FileLoader::readAllSaveSlots(QFile& file) {
    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        readSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (sizeof(SaveSlot) * i));
    }
}

void FileLoader::writeAllSaveSlots(QFile& file) {
    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        writeSaveSlot(file, SaveManager::getInstance()->getSaveSlot(i), getRawDataOffsetStart() + (sizeof(SaveSlot) * i));
    }
}

const SaveData& FileLoader::parseSaveData(QDataStream& inputStream, unsigned int startOffset) {
    SaveData* currentSave = new SaveData();

    // For .note files, the raw save data starts at offset 0x30. We start from there
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
    currentSave->save_crystal_number = readData<unsigned char>(inputStream, inputStream.device()->pos());

    currentSave->field50_0xb1 = readData<unsigned char>(inputStream, inputStream.device()->pos());
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
                 << saveData.field50_0xb1
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

    inputStream.setByteOrder(QDataStream::BigEndian);

    T value;
    inputStream.readRawData(reinterpret_cast<char*>(&value), sizeof(T));

    // Ensure we're reading the data in big endian
    return qFromBigEndian(value);
}
