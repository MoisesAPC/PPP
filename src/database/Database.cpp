#include "include/database/Database.h"
#include "include/database/DatabaseManager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

bool DatabaseCouch::connectToDatabase() {
    QUrl url(QString("http://%1:%2/").arg(getHostname()).arg(getPort()));
    QNetworkRequest request(url);
    QNetworkReply* reply = DatabaseManager::getInstance()->getNetworkAccessManager()->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool success = (reply->error() == QNetworkReply::NoError);
    reply->deleteLater();

    return success;
}
