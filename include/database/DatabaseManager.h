#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "include/database/Database.h"
#include <QNetworkAccessManager>

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

        Database* getDatabase() {
            return database;
        }

        QNetworkAccessManager* getNetworkAccessManager() {
            return networkAcessManager;
        }

        void clearManager() {
            databaseType = DATABASE_NONE;

            if (database != nullptr) {
                delete database;
                database = nullptr;
            }

            if (networkAcessManager != nullptr) {
                delete networkAcessManager;
                networkAcessManager = nullptr;
            }
        }

        bool connect();
        void closeConnection();

        void findEntry(const QString& id);
        void createEntry(const QString& id, const SaveData& saveData);
        void updateEntry(const QString& id, const SaveData& saveData);
        SaveData& getEntry(const QString& id, const SaveData& saveData) const;
        void deleteEntry(const QString& id);

    private:
        static DatabaseManager* instance;

        DatabaseManager() {
            networkAcessManager = new QNetworkAccessManager();
        }

        ~DatabaseManager() { clearManager(); }
        DatabaseManager(const DatabaseManager& obj) = delete; // Remove the copy constructor

        enum eDatabaseType {
            DATABASE_COUCHDB,
            DATABASE_NONE = -1
        };

        int databaseType = DATABASE_NONE;
        Database* database = nullptr;
        QNetworkAccessManager* networkAcessManager = nullptr;
};

#endif
