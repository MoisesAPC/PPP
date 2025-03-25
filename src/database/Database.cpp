#include "include/database/Database.h"
#include "include/database/DatabaseManager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

bool DatabaseCouch::connectToDatabase() {
    QNetworkAccessManager* networkManager = DatabaseManager::getInstance()->allocNetworkAccessManager();

    QUrl url(QString("http://%1:%2/").arg(getHostname()).arg(getPort()));
    QNetworkRequest request(url);

    // Send the "GET" request and then wait for a responsed (loop.exec())
    // If no errors are gotten, we've connected successully
    QNetworkReply* reply = networkManager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool success = (reply->error() == QNetworkReply::NoError);
    reply->deleteLater();

    return success;
}

void DatabaseCouch::disconnectFromDatabase() {
    DatabaseManager::getInstance()->destroyNetworkAccessManager();
}
