#ifndef DATABASE_H
#define DATABASE_H

#include "include/save/SaveManager.h"

struct Database {
    QString hostname = "";
    int port = 0;

    Database() {}
    virtual ~Database() {}

    void setHostname(const QString& hostname_) { hostname = hostname_; }
    void setPort(const int port_) { port = port_; }
    QString getHostname() const { return hostname; }
    int getPort() const { return port; }

    virtual bool connect() = 0;
};

struct DatabaseCouch: public Database {
    DatabaseCouch() {}
    ~DatabaseCouch() {}

    bool connect();
};

#endif
