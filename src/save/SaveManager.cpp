#include "include/save/SaveManager.h"

short SaveManager::getRegion() const {
    return region;
}

void SaveManager::setRegion(const short region_) {
    region = region_;
}

/*
void SaveManager::setLanguage(const short language) {
    getInstance()->getCurrentSave().language = language;
}
*/
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

void SaveManager::setWhiteJewel(const unsigned short save_crystal_number) {
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

unsigned int SaveManager::calcFirstChecksum(unsigned char* dataFromFile) {
    unsigned int checksum = 0;
    unsigned int offset = 0;
    unsigned char* data;

    for (data = dataFromFile; offset != sizeof(SaveData); offset++) {
        checksum = *data + checksum;
        data++;
    }

    return checksum;
}

unsigned int SaveManager::calcSecondChecksum(unsigned int* dataFromFile) {
    unsigned int checksum = 0;
    unsigned int offset = 0;
    unsigned int* data;

    for (data = dataFromFile; offset != sizeof(SaveData) / sizeof(unsigned int); offset++) {
        checksum = *data ^ checksum;
        data++;
    }

    return checksum;
}
