#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "include/database/Database.h"
#include <QNetworkAccessManager>

class DatabaseManager {
    public:
        enum eDatabaseType {
            DATABASE_COUCHDB,
            DATABASE_NONE = -1
        };

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

        QString getUsername() const {
            return username;
        }

        QString getPassword() const {
            return password;
        }

        void setUsername(const QString& username_) {
            username = username_;
        }

        void setPassword(const QString& password_) {
            password = password_;
        }

        void clearManager() {
            databaseType = DATABASE_NONE;

            if (database != nullptr) {
                delete database;
                database = nullptr;
            }

            destroyNetworkAccessManager();
        }

        QNetworkAccessManager* allocNetworkAccessManager() {
            networkAcessManager = new QNetworkAccessManager();

            return networkAcessManager;
        }

        void destroyNetworkAccessManager() {
            if (networkAcessManager != nullptr) {
                delete networkAcessManager;
                networkAcessManager = nullptr;
            }
        }

        void assignDatabase();

        bool connectToDatabase();
        void disconnectFromDatabase();
        std::vector<Database::SaveBasicInfo> getAllEntries();

        void findEntry(const QString& id);
        void createEntry(const QString& id, const std::vector<SaveSlot>& entry, const QString& rev);
        void getEntry(const QString& id, std::vector<SaveSlot>& array);
        void deleteEntry(const QString& id, const QString& rev);
        bool entryAlreadyExists(const QString& id);
        QString getDocumentRevision(const QString& documentId);

    private:
        static DatabaseManager* instance;

        DatabaseManager() {}
        ~DatabaseManager() { clearManager(); }
        DatabaseManager(const DatabaseManager& obj) = delete; // Remove the copy constructor

        int databaseType = DATABASE_NONE;
        Database* database = nullptr;
        QNetworkAccessManager* networkAcessManager = nullptr;
        QString username = "";
        QString password = "";
};

#endif
