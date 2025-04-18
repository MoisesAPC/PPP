/**
 * @file SaveManager.cpp
 * @brief SaveManager class source code file
 *
 * This file contains the source code for the SaveManager singleton.
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/save/SaveManager.h"

short SaveManager::getRegion() const {
    return region;
}

void SaveManager::setRegion(const short region_) {
    region = region_;
}

void SaveManager::setLanguage(const short language) {
    getInstance()->getCurrentSave().language = language;
}

short SaveManager::getLanguage() const {
    return getInstance()->getCurrentSave().language;
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
    getInstance()->getCurrentSave().minute = minutes;
}

void SaveManager::setSeconds(const short seconds) {
    getInstance()->getCurrentSave().seconds = seconds;
}

void SaveManager::setMilliseconds(const unsigned short milliseconds) {
    getInstance()->getCurrentSave().milliseconds = milliseconds;
}

void SaveManager::setFramecount(const unsigned int gameplay_framecount) {
    getInstance()->getCurrentSave().gameplay_framecount = gameplay_framecount;
}

unsigned int SaveManager::getFrameCount() const {
    return getInstance()->getCurrentSave().gameplay_framecount;
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

unsigned int SaveManager::getFlags() const {
    return getInstance()->getCurrentSave().flags;
}

void SaveManager::setFlags(const unsigned int flags) {
    BITS_SET(getInstance()->getCurrentSave().flags, flags);
}

void SaveManager::unsetFlags(const unsigned int flags) {
    BITS_UNSET(getInstance()->getCurrentSave().flags, flags);
}

unsigned int SaveManager::getPlayerStatus() const {
    return getInstance()->getCurrentSave().player_status;
}

void SaveManager::setPlayerStatus(const unsigned int status) {
    BITS_SET(getInstance()->getCurrentSave().player_status, status);
}

void SaveManager::unsetPlayerStatus(const unsigned int status) {
    BITS_UNSET(getInstance()->getCurrentSave().player_status, status);
}

/**
 * @brief Set a whole event flag word
 */
void SaveManager::setEventFlags(const int flagSet, const unsigned int flags) {
    getInstance()->getCurrentSave().event_flags[flagSet] = flags;
}

/**
 * @brief Assign individual flags to an event flag word.
 */
void SaveManager::assignEventFlags(const int flagSet, const unsigned int flags) {
    BITS_SET(getInstance()->getCurrentSave().event_flags[flagSet], flags);
}

/**
 * @brief Remove individual flags from an event flag word.
 */
void SaveManager::unassignEventFlags(const int flagSet, const unsigned int flags) {
    BITS_UNSET(getInstance()->getCurrentSave().event_flags[flagSet], flags);
}

/**
 * @brief Calculates the save file's first checksum
 *
 * @note source: https://decomp.me/scratch/6UIVU
 */
unsigned int SaveManager::calcFirstChecksum(const QByteArray& dataFromFile) {
    unsigned int checksum = 0;

    unsigned int numElements = dataFromFile.size();
    const unsigned char* data = reinterpret_cast<const unsigned char*>(dataFromFile.constData());

    for (int i = 0; i < numElements; i++) {
        checksum += data[i];
    }

    return checksum;
}

/**
 * @brief Calculates the save file's second checksum
 *
 * @note source: https://decomp.me/scratch/oi0s4
 */
unsigned int SaveManager::calcSecondChecksum(const QByteArray& dataFromFile) {
    unsigned int checksum = 0;

    unsigned int numElements = dataFromFile.size() / 4;
    const unsigned int* data = reinterpret_cast<const unsigned int*>(dataFromFile.constData());

    for (int i = 0; i < numElements; i++) {
        checksum ^= data[i];
    }

    return checksum;
}

/**
 * If none of the saves are enabled, return true.
 * This allows us, for example, to prevent saving if none of the saves's "Enabled" checkbox are checked.
 */
bool SaveManager::areAllSavesDisabled() {
    for (int i = 0; i < NUM_SAVES; i++) {
        if (BITS_HAS(saves[i].mainSave.flags, SaveData::SAVE_FLAG_ACTIVE)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Assign default (i.e. new game) values to all save game fields.
 */
void SaveManager::assignDefaultValues() {
    for (int i = 0; i < NUM_SAVES; i++) {
        saves[i].assignDefaultValues();
    }
}

/**
 * @brief Clears all save game fields.
 */
void SaveManager::clear() {
    for (int i = 0; i < NUM_SAVES; i++) {
        saves[i].clear();
    }
}
