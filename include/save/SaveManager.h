#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

/**
 * @file SaveManager.h
 * @brief SaveManager header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "Save.h"

#include <QFile>
#include <QtEndian>

/**
 * @class SaveManager
 * @brief SaveManager singleton class definition
 *
 * This singleton handles save-related operations.
 */
class SaveManager {
    public:
        // Singleton-related functions
        static SaveManager* getInstance() {
            if (instance == nullptr) {
                createInstance();
            }

            return instance;
        }

        static void createInstance() {
            instance = new SaveManager();
        }

        static void destroyInstance() {
            delete instance;
            instance = nullptr;
        }

        int currentSave = 0;
        bool isMain = true;

        // Getters, setters and helper functions
        void parseRegion(QFile& file);
        short getRegion() const;
        void setRegion(const short);
        void setLanguage(const short);
        short getLanguage() const;
        void setLife(const short);
        void setGold(const unsigned int);
        void setItem(const int, const unsigned char);
        void setSpawn(const short);
        void setWhiteJewel(const unsigned short);
        void setTimesSaved(const unsigned int);
        void setDeathCount(const unsigned int);
        void setGoldRenon(const unsigned int);
        void setHourVamp(const unsigned short);
        void setHealthDepletionRate(const unsigned short);
        void setWeek(const short);
        void setDay(const short);
        void setHour(const short);
        void setMinutes(const short);
        void setSeconds(const short);
        void setMilliseconds(const unsigned short);
        void setFramecount(const unsigned int);
        unsigned int getFrameCount() const;
        void setCharacter(const short);
        void setButtonConfig(const short);
        void setSoundMode(const short);
        void setSubweapon(const short);
        void setMap(const short);
        unsigned int getFlags() const;
        void setFlags(const unsigned int);
        void unsetFlags(const unsigned int);
        void setEventFlags(const int, const unsigned int);
        unsigned int getPlayerStatus() const;
        void setPlayerStatus(const unsigned int status);
        void unsetPlayerStatus(const unsigned int status);
        void assignEventFlags(const int, const unsigned int);
        void unassignEventFlags(const int, const unsigned int);
        unsigned int calcFirstChecksum(const QByteArray&);
        unsigned int calcSecondChecksum(const QByteArray&);
        bool areAllSavesDisabled();

        SaveSlot& getSaveSlot(const int index) {
            return saves[index];
        }

        SaveSlot& getCurrentSaveSlot() {
            return saves[currentSave];
        }

        SaveData& getSave(const int index, const bool isMain) {
            return (isMain) ? getSaveSlot(index).mainSave : getSaveSlot(index).beginningOfStage;
        }

        SaveData& getCurrentSave() {
            return (isMain) ? getSaveSlot(currentSave).mainSave : getSaveSlot(currentSave).beginningOfStage;
        }

        SaveSlot* getAllSaves() {
            return saves;
        }

        void setSaveSlot(const SaveSlot& save, const int index) {
            saves[index] = save;
        }

        void clear();
        void assignDefaultValues();

    private:
        static SaveManager* instance;

        // Constructors and destructor
        SaveManager() {}
        ~SaveManager() {}
        SaveManager(const SaveManager& obj) = delete; // Remove the copy constructor

        SaveSlot saves[NUM_SAVES];
        short region = SaveData::USA;
};

#endif
