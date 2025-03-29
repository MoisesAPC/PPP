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

void DatabaseCouch::createEntry(const QString& id, const std::vector<SaveSlot>& entries, const QString& rev) {
    QUrl url(QString("http://%1:%2/%3/%4").arg(getHostname()).arg(getPort()).arg(getDatabaseName()).arg(id));
    QNetworkRequest request(url);
    createAuthorizationHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Check if the given documentId already exists in the database
    // If true, propmt the replacement window. If yes is selected, proceed with the replacement
    if (entryAlreadyExistsGivenRequest(request)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Confirm overwrite",
                                      "This document ID already exists. Do you want to overwrite it?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }
    }

    // Send the "PUT" request with the JSON object (converted from the SaveSlot) and wait for the reply
    QJsonArray jsonArray;
    for (int i = 0; i < NUM_SAVES; i++) {
        jsonArray.append(parseSaveSlotToJSON(entries[i]));
    }

    // Put the json array into the jsonObject "saves". This is what will be sent to the database.
    QJsonObject jsonDoc;
    jsonDoc["saves"] = jsonArray;

    // When replacing (NOT creating) saves, ensure to pass "rev" in order to properly update the document.
    // For newly created files, this is not needed.
    if (!rev.isEmpty()) {
        jsonDoc["_rev"] = rev;
    }

    QNetworkReply* reply = DatabaseManager::getInstance()->getNetworkAccessManager()->put(request, QJsonDocument(jsonDoc).toJson());

    waitForEventToFinish(reply);
    // When the request is finished, call "getDatabaseRequestReply" to get the response (either success or error)
    getDatabaseRequestReply(reply);

    reply->deleteLater();
}

bool DatabaseCouch::entryAlreadyExistsGivenRequest(const QNetworkRequest& request) {
    QNetworkReply* checkReply = DatabaseManager::getInstance()->getNetworkAccessManager()->get(request);
    waitForEventToFinish(checkReply);

    bool documentExists = (checkReply->error() == QNetworkReply::NoError);
    checkReply->deleteLater();

    return documentExists;
}

bool DatabaseCouch::entryAlreadyExists(const QString& id) {
    QUrl url(QString("http://%1:%2/%3/%4").arg(getHostname()).arg(getPort()).arg(getDatabaseName()).arg(id));
    QNetworkRequest request(url);
    createAuthorizationHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    return entryAlreadyExistsGivenRequest(request);
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

    return json;
}

SaveData DatabaseCouch::parseJSONToSaveData(const QJsonObject& json) {
    SaveData saveData = {};

    QJsonArray eventFlagsArray = json["event_flags"].toArray();
    for (int i = 0; i < NUM_EVENT_FLAGS; i++) {
        saveData.event_flags[i] = eventFlagsArray[i].toInt();
    }

    saveData.flags = json["flags"].toInt();
    saveData.week = static_cast<short>(json["week"].toInt());
    saveData.day = static_cast<short>(json["day"].toInt());
    saveData.hour = static_cast<short>(json["hour"].toInt());
    saveData.minute = static_cast<short>(json["minute"].toInt());
    saveData.seconds = static_cast<short>(json["seconds"].toInt());
    saveData.milliseconds = static_cast<unsigned short>(json["milliseconds"].toInt());
    saveData.gameplay_framecount = json["gameplay_framecount"].toInt();
    saveData.button_config = static_cast<short>(json["button_config"].toInt());
    saveData.sound_mode = static_cast<short>(json["sound_mode"].toInt());
    saveData.language = static_cast<short>(json["language"].toInt());
    saveData.character = static_cast<short>(json["character"].toInt());
    saveData.life = static_cast<short>(json["life"].toInt());
    saveData.subweapon = static_cast<short>(json["subweapon"].toInt());
    saveData.gold = static_cast<unsigned int>(json["gold"].toInt());

    QJsonArray itemsArray = json["items"].toArray();
    for (int j = 0; j < SIZE_ITEMS_ARRAY; j++) {
        saveData.items[j] = static_cast<unsigned char>(itemsArray[j].toInt());
    }

    saveData.player_status = json["player_status"].toInt();
    saveData.health_depletion_rate_while_poisoned = static_cast<short>(json["health_depletion_rate_while_poisoned"].toInt());
    saveData.current_hour_VAMP = static_cast<unsigned short>(json["current_hour_VAMP"].toInt());
    saveData.map = static_cast<short>(json["map"].toInt());
    saveData.spawn = static_cast<short>(json["spawn"].toInt());
    saveData.save_crystal_number = static_cast<unsigned short>(json["save_crystal_number"].toInt());
    saveData.time_saved_counter = json["time_saved_counter"].toInt();
    saveData.death_counter = json["death_counter"].toInt();
    saveData.gold_spent_on_Renon = json["gold_spent_on_Renon"].toInt();

    return saveData;
}

QJsonObject DatabaseCouch::parseSaveSlotToJSON(const SaveSlot& saveSlot) {
    QJsonObject json;

    json["mainSave"] = parseSaveDataToJSON(saveSlot.mainSave);
    json["beginningOfStage"] = parseSaveDataToJSON(saveSlot.beginningOfStage);
    json["checksum1"] = static_cast<int>(saveSlot.checksum1);
    json["checksum2"] = static_cast<int>(saveSlot.checksum2);
    json["region"] = SaveManager::getInstance()->getRegion();

    return json;
}

SaveSlot DatabaseCouch::parseJSONToSaveSlot(const QJsonObject& json) {
    SaveSlot saveSlot;

    saveSlot.mainSave = parseJSONToSaveData(json["mainSave"].toObject());
    saveSlot.beginningOfStage = parseJSONToSaveData(json["beginningOfStage"].toObject());
    saveSlot.checksum1 = static_cast<unsigned int>(json["checksum1"].toInt());
    saveSlot.checksum2 = static_cast<unsigned int>(json["checksum2"].toInt());
    SaveManager::getInstance()->setRegion(static_cast<short>(json["region"].toInt()));

    return saveSlot;
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
    entries.clear();

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

    for (const QJsonValue& value: rows) {
        QJsonObject rowObj = value.toObject();
        QString docId = rowObj["id"].toString();

        QJsonObject docObj = rowObj["doc"].toObject();
        if (!docObj.isEmpty()) {
            QString rev = docObj["_rev"].toString();

            QJsonArray savesArray = docObj["saves"].toArray();
            if (!savesArray.isEmpty()) {
                QJsonObject firstSave = savesArray.first().toObject();
                int region = firstSave["region"].toInt();
                entries.push_back({docId, rev, region});
            }
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

// Obtains the document's associated "rev" given the documentId
QString DatabaseCouch::getDocumentRevision(const QString& documentId) {
    QUrl url(QString("http://%1:%2/%3/%4")
                 .arg(getHostname())
                 .arg(getPort())
                 .arg(getDatabaseName())
                 .arg(documentId));

    QNetworkRequest request(url);
    createAuthorizationHeader(request);

    QNetworkReply* reply = DatabaseManager::getInstance()->getNetworkAccessManager()->get(request);
    waitForEventToFinish(reply);

    QString rev = "";
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        if (jsonResponse.isObject() && jsonResponse.object().contains("_rev")) {
            rev = jsonResponse.object()["_rev"].toString();
        }
    }

    reply->deleteLater();
    return rev;
}

void DatabaseCouch::getEntry(const QString& id, std::vector<SaveSlot>& array) {
    array.clear();

    QUrl url(QString("http://%1:%2/%3/%4").arg(getHostname()).arg(getPort()).arg(getDatabaseName()).arg(id));
    QNetworkRequest request(url);
    createAuthorizationHeader(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = DatabaseManager::getInstance()->getNetworkAccessManager()->get(request);
    waitForEventToFinish(reply);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject jsonObject = jsonDoc.object();
            QJsonArray savesArray = jsonObject["saves"].toArray();

            for (const QJsonValue& saveSlot: savesArray) {
                if (saveSlot.isObject()) {
                    array.push_back(parseJSONToSaveSlot(saveSlot.toObject()));
                }
            }
        }
    }

    reply->deleteLater();
}
