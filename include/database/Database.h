#ifndef DATABASE_H
#define DATABASE_H

#include "include/save/SaveManager.h"
#include <QObject>

struct Database: public QObject {
    Q_OBJECT

    QString hostname = "";
    int port = 0;
    QString databaseName = "";

    // @note Wee need to explicitly declare the constructor and virtual destructor as "public"
    // since "QObject" (needed for the "connect" function to work with this struct)
    // has a private destructor, which throws errors
    public:
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
};

struct DatabaseCouch: public Database {
    DatabaseCouch() {}
    ~DatabaseCouch() {}

    bool connectToDatabase();
    void disconnectFromDatabase();
};

#endif
