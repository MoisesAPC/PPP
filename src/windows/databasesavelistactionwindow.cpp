#include "include/windows/databaseaccess/databasesavelistactionwindow.h"
#include "ui_databasesavelistactionwindow.h"

DatabaseSaveListActionWindow::DatabaseSaveListActionWindow(const QString& docId, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseSaveListActionWindow)
    , documentId(docId)
{
    ui->setupUi(this);
}

DatabaseSaveListActionWindow::~DatabaseSaveListActionWindow() {
    delete ui;
}
