#ifndef DATABASE_H
#define DATABASE_H

#include "include/save/SaveManager.h"
#include <QObject>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>

// We inherit from QObject in order to be able to use the "connect" function using this class
struct Database: public QObject {
    Q_OBJECT

    QString hostname = "";
    int port = 0;
    QString databaseName = "";

    // @note Wee need to explicitly declare the constructor and virtual destructor as "public"
    // since "QObject" (needed for the "connect" function to work with this struct)
    // has a private destructor, which throws errors
    public:
        struct SaveBasicInfo {
            QString documentId = "";
            QString rev = "";   // Revisional info needed for delete operations to work correctly
            int region = SaveData::USA;
        };

        Database() {}
        virtual ~Database() {}

        void setHostname(const QString& hostname_) { hostname = hostname_; }
        void setPort(const int port_) { port = port_; }
        QString getHostname() const { return hostname; }
        int getPort() const { return port; }
        void setDatabaseName(const QString& databaseName_) { databaseName = databaseName_; }
        QString getDatabaseName() const { return databaseName; }

        virtual bool connectToDatabase() = 0;
        virtual void disconnectFromDatabase() = 0;
        virtual void createEntry(const QString& id, const SaveData& saveData, const QString& rev) = 0;
        virtual void deleteEntry(const QString& id, const QString& rev) = 0;
        virtual std::vector<Database::SaveBasicInfo> getAllEntries() = 0;
        virtual void parseGetAllEntriesResponse(const QByteArray& data, std::vector<Database::SaveBasicInfo>& entries) = 0;
        virtual bool entryAlreadyExists(const QString& id) = 0;
        virtual QString getDocumentRevision(const QString& documentId) = 0;

        void waitForEventToFinish(QNetworkReply* reply);
};

struct DatabaseCouch: public Database {
    DatabaseCouch() {}
    ~DatabaseCouch() {}

    bool connectToDatabase();
    void disconnectFromDatabase();
    void createEntry(const QString& id, const SaveData& saveData, const QString& rev);
    bool entryAlreadyExistsGivenRequest(const QNetworkRequest& request);
    bool entryAlreadyExists(const QString& id);
    QString getDocumentRevision(const QString& documentId);

    void getDatabaseRequestReply(QNetworkReply* reply);
    QJsonObject parseSaveDataToJSON(const SaveData&);
    void createAuthorizationHeader(QNetworkRequest& request);
    std::vector<Database::SaveBasicInfo> getAllEntries();
    void parseGetAllEntriesResponse(const QByteArray& data, std::vector<Database::SaveBasicInfo>& entries);
    void deleteEntry(const QString& id, const QString& rev);
};

#endif
