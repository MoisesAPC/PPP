#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "include/database/Database.h"

class DatabaseManager {
    public:
        static DatabaseManager* getInstance() {
            if (instance == nullptr) {
                createInstance();
            }

            return instance;
        }

        static void createInstance() {
            instance = new DatabaseManager();
        }

        static void destroyInstance() {
            delete instance;
            instance = nullptr;
        }

        int getDatabaseType() const {
            return databaseType;
        }

        void setDatabaseType(const int databaseType_) {
            databaseType = databaseType_;
        }

        void clearManager() {
            databaseType = DATABASE_NONE;

            if (database != nullptr) {
                delete database;
                database = nullptr;
            }
        }

        void connect();
        void closeConnection();

        void findEntry(const QString& id);
        void createEntry(const QString& id, const SaveData& saveData);
        void updateEntry(const QString& id, const SaveData& saveData);
        SaveData& getEntry(const QString& id, const SaveData& saveData) const;
        void deleteEntry(const QString& id);

    private:
        static DatabaseManager* instance;

        DatabaseManager() {
            database = new Database();
        }

        ~DatabaseManager() { clearManager(); }
        DatabaseManager(const DatabaseManager& obj) = delete; // Remove the copy constructor

        enum eDatabaseType {
            DATABASE_COUCHDB,
            DATABASE_NONE = -1
        };

        int databaseType = DATABASE_NONE;
        Database* database = nullptr;
};

#endif
