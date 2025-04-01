#ifndef DATABASEACCESSWINDOW_H
#define DATABASEACCESSWINDOW_H

/**
 * @file DatabaseMainWindow.h
 * @brief DatabaseMainWindow header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/windows/ComboBoxData.h"
#include "include/database/DatabaseManager.h"

#include <QDialog>
#include <QComboBox>
#include <QStackedWidget>

namespace Ui {
class DatabaseMainWindow;
}

/**
 * @class DatabaseMainWindow
 * @brief Main database window
 *
 * Main window that is opened in the File > Database menu
 */
class DatabaseMainWindow : public QDialog
{
    Q_OBJECT

public:
    /**< The number of save buttons rendered on the list */
    const int entriesPerPage = 5;

    explicit DatabaseMainWindow(QWidget* parent = nullptr);
    ~DatabaseMainWindow();

private:
    Ui::DatabaseMainWindow* ui;

    // Data for this window's combo boxes
    const Ui::ComboBoxData comboBoxDataDatabaseTypes = {
        {{"CouchDB", DatabaseManager::DATABASE_COUCHDB}}
    };

    // Setup functions
    void setupConnectMenu();
    void setupSaveListMenu();
    void createSaveList();
    void createSaveListButtons();
    void setSaveListButtonProperties(QPushButton* button, const QString& documentId, const int listIndex, const int region, const QString& rev);
    void setupComboBox(QComboBox* comboBox, const Ui::ComboBoxData& array, std::function<void(int)> setter);
    void setupLineEditHostname(QLineEdit* lineEdit);

    // Interface event handling functions
    void onPageSwitch();
    void switchPage(QStackedWidget* stackedWidget, const QWidget* page);
    void onConnectButtonPress();
    void onActionButtonClicked(const QString& docId, const QString& rev);
    void onUploadSaveButtonPress();

    // Helper functions
    void clearSaveList();

    /**< An array with the variables from the save used to display each entry in the save list buttons */
    std::vector<Database::SaveBasicInfo> saveEntries;
};

#endif // DATABASEACCESSWINDOW_H
