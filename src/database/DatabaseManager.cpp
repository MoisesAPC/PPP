#include "include/database/DatabaseManager.h"

bool DatabaseManager::connect() {
    if (database != nullptr) {
        return database->connect();
    }

    return false;
}
