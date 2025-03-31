#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

/**
 * @file DatabaseManager.h
 * @brief DatabaseManager header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/database/Database.h"
#include <QNetworkAccessManager>

/**
 * @class DatabaseManager
 * @brief DatabaseManager singleton class definition
 *
 * This singleton handles database-related operations.
 * This class is a wrapper for database tasks. The specific operations are handled
 * by the currently-loaded database (see the "database" member variable).
 */
class DatabaseManager {
    public:
        enum eDatabaseType {
            DATABASE_COUCHDB,
            DATABASE_NONE = -1
        };

        // Singleton-related functions
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

        // Inline getters and setters
        inline int getDatabaseType() const {
            return databaseType;
        }

        inline void setDatabaseType(const int databaseType_) {
            databaseType = databaseType_;
        }

        inline Database* getDatabase() {
            return database;
        }

        inline QNetworkAccessManager* getNetworkAccessManager() {
            return networkAcessManager;
        }

        inline QString getUsername() const {
            return username;
        }

        inline QString getPassword() const {
            return password;
        }

        inline void setUsername(const QString& username_) {
            username = username_;
        }

        inline void setPassword(const QString& password_) {
            password = password_;
        }

        // Helper functions
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

        void clearManager() {
            databaseType = DATABASE_NONE;

            if (database != nullptr) {
                delete database;
                database = nullptr;
            }

            destroyNetworkAccessManager();
        }

        // Database tasks
        bool connectToDatabase();
        void disconnectFromDatabase();
        void assignDatabase();
        void getEntry(const QString& id, std::vector<SaveSlot>& array);
        std::vector<Database::SaveBasicInfo> getAllEntries();
        void createEntry(const QString& id, const std::vector<SaveSlot>& entry, const QString& rev);
        void deleteEntry(const QString& id, const QString& rev);
        bool entryAlreadyExists(const QString& id);
        QString getDocumentRevision(const QString& documentId);

    private:
        static DatabaseManager* instance;

        // Constructors and destructor
        DatabaseManager() {}
        ~DatabaseManager() { clearManager(); }
        DatabaseManager(const DatabaseManager& obj) = delete; // Remove the copy constructor

        int databaseType = DATABASE_NONE;

        /**
         * @brief database
         *
         * Stores the current "Database" class instance.
         * Depending on the database type, this will allocate
         * a inherited class of Database, which will perform the operations
         * specific to said database type.
         * (for example: for CouchDB, "DatabaseCouch" will be instantiated here).
         */
        Database* database = nullptr;
        /**
         * @brief networkAcessManager
         *
         * networkAcessManager is mandatory to perform database-operations.
         * Since we only need one instance of it, we store it as a member variable
         * and just create it / delete it whenever we need it.
         */
        QNetworkAccessManager* networkAcessManager = nullptr;

        /**
         * Credientials for connecting to the database.
         */
        QString username = "";
        QString password = "";
};

#endif
