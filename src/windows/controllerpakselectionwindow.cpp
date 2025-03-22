#include "include\windows\controllerpakselection\controllerpakselectionwindow.h"

ControllerPakSelectionWindow::ControllerPakSelectionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ControllerPakSelectionWindow)
{
    ui->setupUi(this);
}

ControllerPakSelectionWindow::~ControllerPakSelectionWindow()
{
    delete ui;
}
