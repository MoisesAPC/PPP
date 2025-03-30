#ifndef DATABASESAVELISTACTIONWINDOW_H
#define DATABASESAVELISTACTIONWINDOW_H

/******************************************************************************
 * DatabaseSaveListActionButtonWindow.h
 *
 * Header file for DatabaseSaveListActionButtonWindow.cpp
 *****************************************************************************/

#include <QDialog>

namespace Ui {
class DatabaseSaveListActionWindow;
}

class DatabaseSaveListActionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSaveListActionWindow(const QString& docId, const QString& rev, QWidget* parent = nullptr);
    ~DatabaseSaveListActionWindow();

// Using signals, we can notify the parent window (DatabaseAccessWindow) that we have finished performing an action
// either delete or edit
signals:
    // If true, deletion was successfull - go back to the save list
    // Otherwise, "No" was pressed - dont exit this window
    void deleteConfirmed(bool goBackToSaveListMenu);

    // If true, we confirmed that we want to edit the current save - go back to the main window
    // Otherwise, "No" was pressed - dont exit this window
    void editConfirmed(bool goBackToMainWindow);

private:
    Ui::DatabaseSaveListActionWindow* ui;
    QString documentId = "";
    QString rev = "";

    void onEditButton();
    void onDeleteButton();
};

#endif // DATABASESAVELISTACTIONWINDOW_H
