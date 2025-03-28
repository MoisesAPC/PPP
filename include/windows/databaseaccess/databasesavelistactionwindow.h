#ifndef DATABASESAVELISTACTIONWINDOW_H
#define DATABASESAVELISTACTIONWINDOW_H

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

private:
    Ui::DatabaseSaveListActionWindow* ui;
    QString documentId = "";
    QString rev = "";

    void onEditButton();
    void onDeleteButton();
};

#endif // DATABASESAVELISTACTIONWINDOW_H
