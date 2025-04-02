/**
 * @file DatabaseSaveListActionButtonWindow.cpp
 * @brief DatabaseSaveListActionButtonWindow class source code file
 *
 * This file contains the source code for the small window that opens
 * when selecting a save file in the database main window.
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/windows/Database/DatabaseSaveListActionButtonWindow.h"
#include "ui_DatabaseSaveListActionButtonWindow.h"
#include "include/windows/main/MainWindow.h"
#include "include/database/DatabaseManager.h"
#include "include/file/FileManager.h"
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
    std::vector<SaveSlot> entries;
    entries.clear();

    // If the user was already editing a save (i.e. if at least one save is Enabled), prompt if they really want to overwrite
    // their changes with the save obtained from the database
    if (!SaveManager::getInstance()->areAllSavesDisabled()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Overwrite changes", "Are you sure you want to edit this file?\n"
                                                                                             "The unsaved changes will be lost.",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            emit editConfirmed(false);
            return;
        }
    }

    // Ensure we can't use the regular "Save" button (since this save wasn't obtained by opening a file)
    // Therefore, only the "Save As..." menu should be available
    FileManager::getInstance()->setFileOpened(false);

    DatabaseManager::getInstance()->getEntry(documentId, entries);

    for (int i = 0; i < entries.size(); i++) {
        SaveManager::getInstance()->setSaveSlot(entries[i], i);
    }

    emit editConfirmed(true);
    documentId = "";
    rev = "";

    // Populate with the first slot by default + main save
    MainWindow::instance->populateMainWindow(&SaveManager::getInstance()->getSaveSlot(0).mainSave);
    MainWindow::instance->updateSlotMenuCheckedState(0, true);
    close();
}

void DatabaseSaveListActionWindow::onDeleteButton() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        DatabaseManager::getInstance()->deleteEntry(documentId, rev);

        // Tell the parent (the database main window), that we've successfully deleted a file,
        // so it can reload the database save list
        emit deleteConfirmed(true);
        documentId = "";
        rev = "";
        close();
    }
    else {
        emit deleteConfirmed(false);
    }
}
