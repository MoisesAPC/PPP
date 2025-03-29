#include "include/windows/databaseaccess/databasesavelistactionwindow.h"
#include "ui_databasesavelistactionwindow.h"
#include "include/database/DatabaseManager.h"
#include <QMessageBox>

DatabaseSaveListActionWindow::DatabaseSaveListActionWindow(const QString& docId, const QString& revision, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseSaveListActionWindow)
    , documentId(docId)
    , rev(revision)
{
    ui->setupUi(this);

    connect(ui->buttonEdit, &QPushButton::clicked, this, &DatabaseSaveListActionWindow::onEditButton);
    connect(ui->buttonDelete, &QPushButton::clicked, this, &DatabaseSaveListActionWindow::onDeleteButton);
}

DatabaseSaveListActionWindow::~DatabaseSaveListActionWindow() {
    delete ui;
}

void DatabaseSaveListActionWindow::onEditButton() {
    // If the user was already editing a save (i.e. if at least one save is Enabled), prompt if they really want to overwrite
    // their changes with the save obtained from the database
    if (!SaveManager::getInstance()->areAllSavesDisabled()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Overwrite changes", "Are you sure you want to edit this file?\n"
                                                                                             "The unsaved changes will be lost.",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    for (int i = 0; i < NUM_SAVES; i++) {
        //DatabaseManager::getInstance()->getEntry(documentId, SaveManager::getInstance()->getSaveSlot(i));
    }
}

void DatabaseSaveListActionWindow::onDeleteButton() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        DatabaseManager::getInstance()->deleteEntry(documentId, rev);

        emit deleteConfirmed(true);
        documentId = "";
        rev = "";
        close();
    }
    else {
        emit deleteConfirmed(false);
    }
}
