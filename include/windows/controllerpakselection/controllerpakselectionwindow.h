#ifndef CONTROLLERPAKSELECTIONWINDOW_H
#define CONTROLLERPAKSELECTIONWINDOW_H

#include "ui_controllerpakselectionwindow.h"
#include <QDialog>

namespace Ui {
class ControllerPakSelectionWindow;
}

class ControllerPakSelectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ControllerPakSelectionWindow(QWidget *parent = nullptr);
    ~ControllerPakSelectionWindow();

private:
    Ui::ControllerPakSelectionWindow *ui;
};

#endif // CONTROLLERPAKSELECTIONWINDOW_H
