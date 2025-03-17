#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "Save.h"

#include <QFile>
#include <QtEndian>
#include <iostream>

class SaveManager {
    public:
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

        const SaveData& parseSaveData(QDataStream& inputStream, long startOffset);
        void parseSaveSlot(QFile& file, SaveSlot& slot, long startOffset);
        void parseAllSaveSlots(QFile& file, long startOffset);

        void setLife(const short);
        void setGold(const unsigned int);
        void setItem(const int, const unsigned char);
        void setSpawn(const short);
        void setWhiteJewel(const unsigned char);
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
        void setCharacter(const short);
        void setButtonConfig(const short);
        void setSoundMode(const short);
        void setSubweapon(const short);
        void setMap(const short);
        void setFlags(const unsigned int);
        void unsetFlags(const unsigned int);

        template<typename T>
        T readData(QDataStream& inputStream, long offset) {
            inputStream.device()->seek(offset);

            inputStream.setByteOrder(QDataStream::BigEndian);

            T value;
            inputStream.readRawData(reinterpret_cast<char*>(&value), sizeof(T));

            // Ensure we're reading the data in big endian
            return qFromBigEndian(value);
        }

        template<typename T>
        void writeData(QDataStream& outputStream, const long offset, T value) {
            outputStream.device()->seek(offset);
            outputStream << qFromBigEndian(value);
        }

        SaveSlot& getSaveSlot(const int index) {
            return saves[index];
        }

        SaveData& getSave(const int index, const bool isMain) {
            return (isMain) ? getSaveSlot(index).main : getSaveSlot(index).beginningOfStage;
        }

        SaveData& getCurrentSave() {
            return (isMain) ? getSaveSlot(currentSave).main : getSaveSlot(currentSave).beginningOfStage;
        }

        SaveSlot* getAllSaves() {
            return saves;
        }

        void setSave(const SaveSlot& save, const int index) {
            saves[index] = save;
        }

        void createSlotMenu();

        void printAllSaves() {
            SaveManager* saveManager = SaveManager::getInstance();
            const SaveSlot* allSaves = saveManager->getAllSaves();

            for (int i = 0; i < NUM_SAVES; i++) {
                const SaveSlot& slot = allSaves[i];

                std::cout << "Save Slot " << i + 1 << ":\n";
                std::cout << "--------------------\n";

                printSaveData(slot.main, "Main Save");

                printSaveData(slot.beginningOfStage, "Beginning of Stage Save");

                std::cout << "Checksum 1: " << slot.checksum1 << "\n";
                std::cout << "Checksum 2: " << slot.checksum2 << "\n";

                std::cout << "\n";
            }
        }

        void printSaveData(const SaveData& data, const std::string& title) {
            std::cout << title << ":\n";
            std::cout << "  Character: " << (data.character == SaveData::REINHARDT ? "Reinhardt" : "Carrie") << "\n";
            std::cout << "  Life: " << data.life << "\n";
            std::cout << "  Subweapon: " << data.subweapon << "\n";
            std::cout << "  Gold: " << data.gold << "\n";
            std::cout << "  Map: " << data.map << "\n";
            std::cout << "  Spawn: " << data.spawn << "\n";
            std::cout << "  Time: " << data.week << "w " << data.day << "d "
                      << data.hour << ":" << data.minute << ":" << data.seconds << "\n";
            std::cout << "  Gameplay Frame Count: " << data.gameplay_framecount << "\n";
            std::cout << "  Death Counter: " << data.death_counter << "\n";
        }


    private:
        static SaveManager* instance;

        SaveManager() {}
        ~SaveManager() {}
        SaveManager(const SaveManager& obj) = delete; // Remove the copy constructor

        SaveSlot saves[NUM_SAVES];
};

#endif
