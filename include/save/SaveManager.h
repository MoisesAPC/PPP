#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "Save.h"

#include <QFile>
#include <QtEndian>


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

        void parseNote(QFile& file);

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

        const SaveData& getSaves(const int index) const {
            return saves[index];
        }

        const SaveData* getAllSaves() const {
            return saves;
        }

        void setSave(const SaveData& save, const int index) {
            saves[index] = save;
        }

    private:
        static SaveManager* instance;

        SaveManager() {}
        ~SaveManager() {}
        SaveManager(const SaveManager& obj) = delete; // Remove the copy constructor

        SaveData saves[NUM_SAVES];
};

#endif
