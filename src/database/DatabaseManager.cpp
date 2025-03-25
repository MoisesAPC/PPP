#include "include/database/DatabaseManager.h"

bool DatabaseManager::connectToDatabase() {
    if (database != nullptr) {
        return database->connectToDatabase();
    }

    return false;
}
