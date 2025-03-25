#include "include\windows\databaseaccess\databaseaccesswindow.h"
#include "ui_databaseaccesswindow.h"

DatabaseAccessWindow::DatabaseAccessWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseAccessWindow)
{
    ui->setupUi(this);
}

DatabaseAccessWindow::~DatabaseAccessWindow()
{
    delete ui;
}
