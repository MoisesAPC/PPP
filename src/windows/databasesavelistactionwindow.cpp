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

}

void DatabaseSaveListActionWindow::onDeleteButton() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete?", QMessageBox::Yes | QMessageBox::No);

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
