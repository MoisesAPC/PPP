#include "include/database/Database.h"
#include "include/database/DatabaseManager.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QMessageBox>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonArray>

// IMPORTANT: We need this code in order to ensure that the "reply" operation (for example, creating a new database entry, deleting, etc)
// is finished BEFORE the code below gets executed (i.e. synchronously)
void Database::waitForEventToFinish(QNetworkReply* reply) {
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

bool DatabaseCouch::connectToDatabase() {
    DatabaseManager* databaseManager = DatabaseManager::getInstance()->getInstance();
    QNetworkAccessManager* networkManager = databaseManager->allocNetworkAccessManager();

    QUrl url(QString("http://%1:%2/").arg(getHostname()).arg(getPort()));
    QNetworkRequest request(url);
    createAuthorizationHeader(request);

    // Send the "GET" request and then wait for a responsed (loop.exec())
    // If no errors are gotten, we've connected successully
    QNetworkReply* reply = networkManager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool success = (reply->error() == QNetworkReply::NoError);

    if (!success) {
        QMessageBox::critical(nullptr, "Error", "An error has occurred while performing this operation.\n"
                                                "Error:" + reply->errorString() + "\n" +
                                                "Response:" + reply->readAll());
    }

    reply->deleteLater();

    return success;
}

void DatabaseCouch::disconnectFromDatabase() {
    DatabaseManager::getInstance()->destroyNetworkAccessManager();
}

void DatabaseCouch::createEntry(const QString &id, const SaveData &saveData) {
    QUrl url(QString("http://%1:%2/%3/%4").arg(getHostname()).arg(getPort()).arg(getDatabaseName()).arg(id));
    QNetworkRequest request(url);
    createAuthorizationHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Send the "PUT" request with the JSON object (converted from the SaveData) and wait for the reply
    QJsonObject jsonDoc = parseSaveDataToJSON(saveData);
    QNetworkReply* reply = DatabaseManager::getInstance()->getNetworkAccessManager()->put(request, QJsonDocument(jsonDoc).toJson());

    waitForEventToFinish(reply);
    // When the request is finished, call "getDatabaseRequestReply" to get the response (either success or error)
    getDatabaseRequestReply(reply);

    reply->deleteLater();
}

void DatabaseCouch::getDatabaseRequestReply(QNetworkReply* reply) {
    if (reply == nullptr) {
        QMessageBox::critical(nullptr, "", "An error has occurred while performing this operation.");
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QString responseText = jsonResponse.isNull() ? QString(responseData) : jsonResponse.toJson(QJsonDocument::Indented);

    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(nullptr, "Success", "The operation was successful!");
    }
    else {
        QMessageBox::critical(nullptr, "Error", "An error has occurred while performing this operation.\n"
                                                "Response:\n" + responseText);
    }

    reply->deleteLater();
}

QJsonObject DatabaseCouch::parseSaveDataToJSON(const SaveData& saveData) {
    QJsonObject json;
    QJsonArray eventFlagsArray;
    QJsonArray itemsArray;

    for (unsigned int i = 0; i < NUM_EVENT_FLAGS; i++) {
        eventFlagsArray.append(static_cast<int>(saveData.event_flags[i]));
    }

    json["event_flags"] = eventFlagsArray;
    json["flags"] = static_cast<int>(saveData.flags);
    json["week"] = saveData.week;
    json["day"] = saveData.day;
    json["hour"] = saveData.hour;
    json["minute"] = saveData.minute;
    json["seconds"] = saveData.seconds;
    json["milliseconds"] = saveData.milliseconds;
    json["gameplay_framecount"] = static_cast<int>(saveData.gameplay_framecount);
    json["button_config"] = saveData.button_config;
    json["sound_mode"] = saveData.sound_mode;
    json["milliseconds"] = saveData.milliseconds;
    json["language"] = saveData.language;
    json["character"] = saveData.character;
    json["life"] = saveData.life;
    json["subweapon"] = saveData.subweapon;
    json["gold"] = static_cast<int>(saveData.gold);

    for (unsigned int j = 0; j < SIZE_ITEMS_ARRAY; j++) {
        eventFlagsArray.append(static_cast<int>(saveData.items[j]));
    }

    json["items"] = itemsArray;
    json["player_status"] = static_cast<int>(saveData.player_status);
    json["health_depletion_rate_while_poisoned"] = saveData.health_depletion_rate_while_poisoned;
    json["current_hour_VAMP"] = saveData.current_hour_VAMP;
    json["map"] = saveData.map;
    json["spawn"] = saveData.spawn;
    json["save_crystal_number"] = saveData.save_crystal_number;
    json["time_saved_counter"] = static_cast<int>(saveData.time_saved_counter);
    json["death_counter"] = static_cast<int>(saveData.death_counter);
    json["gold_spent_on_Renon"] = static_cast<int>(saveData.gold_spent_on_Renon);
    json["region"] = SaveManager::getInstance()->getRegion();

    return json;
}

// Call this function when doing GET, PUT, etc, so that you are authorized to enter the DB
void DatabaseCouch::createAuthorizationHeader(QNetworkRequest& request) {
    DatabaseManager* databaseManager = DatabaseManager::getInstance();

    // Authenticate the credentials. This is done by appending an "authorization" header with the credentials to the "GET" request
    QString authHeader = "Basic " + QByteArray(QString("%1:%2").arg(databaseManager->getUsername()).arg(databaseManager->getPassword()).toUtf8()).toBase64();
    request.setRawHeader("Authorization", authHeader.toUtf8());
}

std::vector<Database::SaveBasicInfo> DatabaseCouch::getAllEntries() {
    std::vector<Database::SaveBasicInfo> entries;

    QUrl url(QString("http://%1:%2/%3/%4").arg(getHostname()).arg(getPort()).arg(getDatabaseName()).arg("_all_docs"));
    QUrlQuery query;
    query.addQueryItem("include_docs", "true"); // This line is needed to ensure we can get all documents from the database
    url.setQuery(query);

    QNetworkRequest request(url);
    createAuthorizationHeader(request);

    QNetworkReply* reply = DatabaseManager::getInstance()->getNetworkAccessManager()->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        parseGetAllEntriesResponse(responseData, entries);
    }

    reply->deleteLater();
    return entries;
}

void DatabaseCouch::parseGetAllEntriesResponse(const QByteArray& data, std::vector<Database::SaveBasicInfo>& entries) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray rows = jsonObj["rows"].toArray();

    for (const QJsonValue& value : rows) {
        QJsonObject rowObj = value.toObject();
        QString docId = rowObj["id"].toString();

        QJsonObject docObj = rowObj["doc"].toObject();
        if (!docObj.isEmpty()) {
            QString rev = docObj["_rev"].toString();
            int region = docObj["region"].toInt();
            entries.push_back({docId, rev, region});
        }
    }
}

void DatabaseCouch::deleteEntry(const QString& id, const QString& rev) {
    QUrl url(QString("http://%1:%2/%3/%4").arg(getHostname()).arg(getPort()).arg(getDatabaseName()).arg(id));
    // We need to add the "rev" field to ensure the deletion is properly made
    QUrlQuery query;
    query.addQueryItem("rev", rev);
    url.setQuery(query);

    QNetworkRequest request(url);
    createAuthorizationHeader(request);
    QNetworkReply* reply = DatabaseManager::getInstance()->getNetworkAccessManager()->deleteResource(request);

    waitForEventToFinish(reply);
    // When the request is finished, call "getDatabaseRequestReply" to get the response (either success or error)
    getDatabaseRequestReply(reply);

    reply->deleteLater();
}
