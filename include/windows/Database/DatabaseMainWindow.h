#ifndef DATABASEACCESSWINDOW_H
#define DATABASEACCESSWINDOW_H

/******************************************************************************
 * DatabaseMainWindow.h
 *
 * Header file for DatabaseMainWindow.cpp
 *****************************************************************************/

#include "include/windows/ComboBoxData.h"
#include "include/database/DatabaseManager.h"

#include <QDialog>
#include <QComboBox>
#include <QStackedWidget>

namespace Ui {
class DatabaseAccessWindow;
}

class DatabaseAccessWindow : public QDialog
{
    Q_OBJECT

public:
    // The number of save buttons rendered on the list
    const int entriesPerPage = 5;

    explicit DatabaseAccessWindow(QWidget *parent = nullptr);
    ~DatabaseAccessWindow();

private:
    Ui::DatabaseAccessWindow* ui;

    Ui::ComboBoxData comboBoxDataDatabaseTypes = {
        {{"CouchDB", DatabaseManager::DATABASE_COUCHDB}}
    };

    void setupConnectMenu();
    void setupSaveListMenu();
    void createSaveList();
    void createSaveListButtons();
    void setSaveListButtonProperties(QPushButton* button, const QString& documentId, const int listIndex, const int region, const QString& rev);
    void clearSaveList();
    void onPageSwitch();

    void setupComboBox(QComboBox* comboBox, const Ui::ComboBoxData& array, std::function<void(int)> setter);
    void setupLineEditHostname(QLineEdit* lineEdit);
    void switchPage(QStackedWidget* stackedWidget, const QWidget* page);
    void onConnectButtonPress();
    void onActionButtonClicked(const QString& docId, const QString& rev);

    void onUploadSaveButtonPress();

    std::vector<Database::SaveBasicInfo> saveEntries;
};

#endif // DATABASEACCESSWINDOW_H
