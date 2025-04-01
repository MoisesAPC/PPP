#ifndef DATABASESAVELISTACTIONWINDOW_H
#define DATABASESAVELISTACTIONWINDOW_H

/**
 * @file DatabaseSaveListActionButtonWindow.h
 * @brief DatabaseSaveListActionButtonWindow header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include <QDialog>

namespace Ui {
class DatabaseSaveListActionWindow;
}

/**
 * @class DatabaseSaveListActionWindow
 *
 * This window is opened when clicking on a save button in the list with all the saves found in the database.
 * It prompts the user with two options:
 *
 * - Edit: Opens the save file from the database into the main program for editing.
 * - Delete: Deletes the save file from the database.
 */
class DatabaseSaveListActionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSaveListActionWindow(const QString& docId, const QString& rev, QWidget* parent = nullptr);
    ~DatabaseSaveListActionWindow();

// Using signals, we can notify the parent window (DatabaseMainWindow) that we have finished performing an action
// either delete or edit
signals:
    // If true, deletion was successfull - go back to the save list
    // Otherwise, "No" was pressed - don't exit this window
    void deleteConfirmed(bool goBackToSaveListMenu);

    // If true, we confirmed that we want to edit the current save - go back to the main window
    // Otherwise, "No" was pressed - dont exit this window
    void editConfirmed(bool goBackToMainWindow);

private:
    Ui::DatabaseSaveListActionWindow* ui;

    /**< The unique identifier of the save we have selected in the save list menu. */
    QString documentId = "";
    /**< The document's revision of the save we have selected in the save list menu. Needed for some creation and deletion tasks. */
    QString rev = "";

    // Interface event handling functions
    void onEditButton();
    void onDeleteButton();
};

#endif // DATABASESAVELISTACTIONWINDOW_H
