#include "include/database/DatabaseManager.h"

bool DatabaseManager::connectToDatabase() {
    if (database != nullptr) {
        return database->connectToDatabase();
    }

    return false;
}

void DatabaseManager::disconnectFromDatabase() {
    if (database != nullptr) {
        database->disconnectFromDatabase();
    }
}

void DatabaseManager::createEntry(const QString &id, const SaveData &saveData) {
    if (database != nullptr) {
        database->createEntry(id, saveData);
    }
}

std::vector<Database::SaveBasicInfo> DatabaseManager::getAllEntries() {
    if (database != nullptr) {
        return database->getAllEntries();
    }

    return std::vector<Database::SaveBasicInfo>();
}

void DatabaseManager::assignDatabase() {
    if (database != nullptr) {
        delete database;
        database = nullptr;
    }

    switch (databaseType) {
        case DATABASE_COUCHDB:
            database = new DatabaseCouch();
            break;
    }
}
