#include "include/save/SaveManager.h"

#include <QDataStream>
#include <QtEndian>
#include <QDebug>

const SaveData& SaveManager::parseSaveData(QDataStream& inputStream, long startOffset) {
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

void SaveManager::parseSaveSlot(QFile& file, SaveSlot& slot, long startOffset) {
    QDataStream inputStream(&file);

    slot.main = parseSaveData(inputStream, startOffset);
    slot.beginningOfStage = parseSaveData(inputStream, inputStream.device()->pos());
    slot.checksum1 = readData<unsigned int>(inputStream, inputStream.device()->pos());
    slot.checksum2 = readData<unsigned int>(inputStream, inputStream.device()->pos());
}

void SaveManager::parseAllSaveSlots(QFile& file, long startOffset) {
    for (unsigned int i = 0; i < NUM_SAVES; i++) {
        parseSaveSlot(file, saves[i], startOffset + (sizeof(SaveSlot) * i));
    }
}

short SaveManager::getRegion() const {
    return region;
}

void SaveManager::setRegion(const short region_) {
    region = region_;
}

void SaveManager::setLanguage(const short language) {
    getInstance()->getCurrentSave().language = language;
}

void SaveManager::setLife(const short life) {
    getInstance()->getCurrentSave().life = life;
}

void SaveManager::setGold(const unsigned int gold) {
    getInstance()->getCurrentSave().gold = gold;
}

void SaveManager::setItem(const int itemId, const unsigned char amount) {
    getInstance()->getCurrentSave().items[itemId - 1] = amount;
}

void SaveManager::setSpawn(const short spawn) {
    getInstance()->getCurrentSave().spawn = spawn;
}

void SaveManager::setWhiteJewel(const unsigned char save_crystal_number) {
    getInstance()->getCurrentSave().save_crystal_number = save_crystal_number;
}

void SaveManager::setTimesSaved(const unsigned int time_saved_counter) {
    getInstance()->getCurrentSave().time_saved_counter = time_saved_counter;
}

void SaveManager::setDeathCount(const unsigned int death_counter) {
    getInstance()->getCurrentSave().death_counter = death_counter;
}

void SaveManager::setGoldRenon(const unsigned int gold_spent_on_Renon) {
    getInstance()->getCurrentSave().gold_spent_on_Renon = gold_spent_on_Renon;
}

void SaveManager::setHourVamp(const unsigned short current_hour_VAMP) {
    getInstance()->getCurrentSave().current_hour_VAMP = current_hour_VAMP;
}

void SaveManager::setHealthDepletionRate(const unsigned short health_depletion_rate_while_poisoned) {
    getInstance()->getCurrentSave().health_depletion_rate_while_poisoned = health_depletion_rate_while_poisoned;
}

void SaveManager::setWeek(const short week) {
    getInstance()->getCurrentSave().week = week;
}

void SaveManager::setDay(const short day) {
    getInstance()->getCurrentSave().day = day;
}

void SaveManager::setHour(const short hour) {
    getInstance()->getCurrentSave().hour = hour;
}

void SaveManager::setMinutes(const short minutes) {
    getInstance()->getCurrentSave().hour = minutes;
}

void SaveManager::setSeconds(const short seconds) {
    getInstance()->getCurrentSave().seconds = seconds;
}

void SaveManager::setMilliseconds(const unsigned short seconds) {
    getInstance()->getCurrentSave().seconds = seconds;
}

void SaveManager::setFramecount(const unsigned int gameplay_framecount) {
    getInstance()->getCurrentSave().gameplay_framecount = gameplay_framecount;
}

void SaveManager::setCharacter(const short character) {
    getInstance()->getCurrentSave().character = character;
}

void SaveManager::setButtonConfig(const short button_config) {
    getInstance()->getCurrentSave().button_config = button_config;
}

void SaveManager::setSoundMode(const short sound_mode) {
    getInstance()->getCurrentSave().sound_mode = sound_mode;
}

void SaveManager::setSubweapon(const short subweapon) {
    getInstance()->getCurrentSave().subweapon = subweapon;
}

void SaveManager::setMap(const short map) {
    getInstance()->getCurrentSave().map = map;
}

unsigned int* SaveManager::getFlagsPtr() {
    return &getInstance()->getCurrentSave().flags;
}

void SaveManager::setFlags(const unsigned int flags) {
    BITS_SET(getInstance()->getCurrentSave().flags, flags);
}

void SaveManager::unsetFlags(const unsigned int flags) {
    BITS_UNSET(getInstance()->getCurrentSave().flags, flags);
}

void SaveManager::setEventFlags(const int flagSet, const unsigned int flags) {
    getInstance()->getCurrentSave().event_flags[flagSet] = flags;
}

void SaveManager::assignEventFlags(const int flagSet, const unsigned int flags) {
    BITS_SET(getInstance()->getCurrentSave().event_flags[flagSet], flags);
}

void SaveManager::unassignEventFlags(const int flagSet, const unsigned int flags) {
    BITS_UNSET(getInstance()->getCurrentSave().event_flags[flagSet], flags);
}
