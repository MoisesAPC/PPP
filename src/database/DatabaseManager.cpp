/**
 * @file DatabaseManager.cpp
 * @brief DatabaseManager source code file
 *
 * This source code file contains the code for the DatabaseManager singleton.
 *
 * @author Moisés Antonio Pestano Castro
 */

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

void DatabaseManager::createEntry(const QString& id, const std::vector<SaveSlot>& entries, const QString& rev) {
    if (database != nullptr) {
        database->createEntry(id, entries, rev);
    }
}

bool DatabaseManager::entryAlreadyExists(const QString& id) {
    if (database != nullptr) {
        return database->entryAlreadyExists(id);
    }

    return false;
}

QString DatabaseManager::getDocumentRevision(const QString& id) {
    if (database != nullptr) {
        return database->getDocumentRevision(id);
    }

    return "";
}

void DatabaseManager::getEntry(const QString& id, std::vector<SaveSlot>& entries) {
    if (database != nullptr) {
        database->getEntry(id, entries);
    }
}

std::vector<Database::SaveBasicInfo> DatabaseManager::getAllEntries() {
    if (database != nullptr) {
        return database->getAllEntries();
    }

    return std::vector<Database::SaveBasicInfo>();
}

void DatabaseManager::deleteEntry(const QString& id, const QString& rev) {
    if (database != nullptr) {
        database->deleteEntry(id, rev);
    }
}

/**
 * @brief Depending on the database type, assign the appropiate database handling class.
 */
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
