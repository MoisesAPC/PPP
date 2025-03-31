#ifndef DATABASE_H
#define DATABASE_H

/**
 * @file Database.h
 * @brief Database header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/save/SaveManager.h"
#include <QObject>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>

/**
 * @class Database
 * @brief Database abstract class
 *
 * This abstract class contains the default implementation for the Database data structure,
 * which is in charge of managing operations with specific databases.
 *
 * @note We inherit from QObject in order to be able to use the "connect" function using this class
 */
class Database: public QObject {
    Q_OBJECT

    QString hostname = "";      /**< Database connection hostname */
    int port = 0;               /**< Database connection port */
    QString databaseName = "";  /**< Name of the database where the saves will be stored at */

    /// @note Wee need to explicitly declare the constructor and virtual destructor as "public"
    /// since "QObject" (needed for the "connect" function to work with this struct)
    /// has a private destructor, which throws errors
    public:
        struct SaveBasicInfo {
            QString documentId = "";    /**< The entry's unique identifier in the database */
            QString rev = "";           /**< Revisional info needed for delete operations to work correctly */
            int region = SaveData::USA; /**< Save region */
        };

        // Constructors and destructor
        Database() {}
        virtual ~Database() {}

        // Getters and setters
        void setHostname(const QString& hostname_) { hostname = hostname_; }
        void setPort(const int port_) { port = port_; }
        QString getHostname() const { return hostname; }
        int getPort() const { return port; }
        void setDatabaseName(const QString& databaseName_) { databaseName = databaseName_; }
        QString getDatabaseName() const { return databaseName; }
        virtual QString getDocumentRevision(const QString& documentId) = 0;
        virtual void getEntry(const QString& id, std::vector<SaveSlot>& array) = 0;
        virtual std::vector<Database::SaveBasicInfo> getAllEntries() = 0;

        // Connection functions
        virtual bool connectToDatabase() = 0;
        virtual void disconnectFromDatabase() = 0;

        // CRUD-related functions
        virtual void createEntry(const QString& id, const std::vector<SaveSlot>& saveSlot, const QString& rev) = 0;
        virtual void deleteEntry(const QString& id, const QString& rev) = 0;
    private:
        virtual void parseGetAllEntriesResponse(const QByteArray& data, std::vector<Database::SaveBasicInfo>& entries) = 0;

        // Helper functions
    public:
        void waitForEventToFinish(QNetworkReply* reply);
        virtual bool entryAlreadyExists(const QString& id) = 0;
};

/**
 * @class DatabaseCouch
 * @brief Database handling class for CouchDB
 *
 * This class extends from Database, and manages CouchDB-specific tasks.
 */
class DatabaseCouch: public Database {
    public:
        // Constructors and destructors
        DatabaseCouch() {}
        ~DatabaseCouch() {}

        // Connection functions
        bool connectToDatabase();
        void disconnectFromDatabase();

        // Getters and setters
        void getEntry(const QString& id, std::vector<SaveSlot>& array);
        std::vector<Database::SaveBasicInfo> getAllEntries();

        // CRUD-related functions
        void createEntry(const QString& id, const std::vector<SaveSlot>& saveSlot, const QString& rev);
        void deleteEntry(const QString& id, const QString& rev);
    private:
        void parseGetAllEntriesResponse(const QByteArray& data, std::vector<Database::SaveBasicInfo>& entries);

    public:
        // Helper functions
        bool entryAlreadyExists(const QString& id);
        QString getDocumentRevision(const QString& documentId);
    private:
        bool entryAlreadyExistsGivenRequest(const QNetworkRequest& request);
        void createAuthorizationHeader(QNetworkRequest& request);
        void getDatabaseRequestReply(QNetworkReply* reply);

    private:
        // SaveData<->JSON parsing functions
        QJsonObject parseSaveDataToJSON(const SaveData&);
        SaveData parseJSONToSaveData(const QJsonObject& json);
        QJsonObject parseSaveSlotToJSON(const SaveSlot& saveSlot);
        SaveSlot parseJSONToSaveSlot(const QJsonObject& json);
};

#endif
