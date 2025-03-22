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
    bool userClosedWithX = false;

    explicit ControllerPakSelectionWindow(QWidget *parent = nullptr);
    ~ControllerPakSelectionWindow();
    void setupButtonBox();
    QString getRegionName(const short region) const;
    void onButtonClicked(int saveIndex);

private:
    Ui::ControllerPakSelectionWindow* ui;

protected:
    // If exiting this window using the X button, notify that it was exited this way,
    // in order to prevent opening the actual save file
    void closeEvent(QCloseEvent* event) override {
        if (this->result() == QDialog::Rejected) {
            // User clicked the X button
            userClosedWithX = true;
        }

        QDialog::closeEvent(event);
    }
};

#endif // CONTROLLERPAKSELECTIONWINDOW_H
