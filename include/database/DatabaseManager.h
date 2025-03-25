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

        int getServerType() const {
            return serverType;
        }

        void setServerType(const int serverType_) {
            serverType = serverType_;
        }

        void clearManager() {
            serverType = SERVER_NONE;
        }

    private:
        static DatabaseManager* instance;

        DatabaseManager() { clearManager(); }
        ~DatabaseManager() {}
        DatabaseManager(const DatabaseManager& obj) = delete; // Remove the copy constructor

        enum eServerType {
            SERVER_COUCHDB,
            SERVER_NONE = -1
        };

        int serverType = SERVER_NONE;
};

#endif
