#ifndef CONTROLLERPAKSELECTIONWINDOW_H
#define CONTROLLERPAKSELECTIONWINDOW_H

/**
 * @file ControllerPakSelectionWindow.h
 * @brief ControllerPakSelectionWindow header file
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "ui_ControllerPakSelectionwindow.h"
#include <QDialog>

namespace Ui {
class ControllerPakSelectionWindow;
}

/**
 * @class ControllerPakSelectionWindow
 * @brief Save list selection window for Controller Pak saves
 *
 * Controller Pak-formatted files are a container of saves for different games.
 * This window lists only the Castlevania 64 saves found in the container, and lets you able to select
 * one for opening it for edit.
 */
class ControllerPakSelectionWindow : public QDialog
{
    Q_OBJECT

public:
    bool userClosedWithX = false;

    // Constructors and destructor
    explicit ControllerPakSelectionWindow(QWidget *parent = nullptr);
    ~ControllerPakSelectionWindow();

    // Setup functions
    void setupButtonBox();

    // Interface event handling functions
    void onButtonClicked(int saveIndex);

    // Helper functions
    QString getRegionName(const short region) const;

private:
    Ui::ControllerPakSelectionWindow* ui;

protected:
    /**
     * If exiting this window using the X button, notify that it was exited this way,
     * in order to prevent opening the actual save file.
     */
    void closeEvent(QCloseEvent* event) override {
        if (this->result() == QDialog::Rejected) {
            // User clicked the X button to exit
            userClosedWithX = true;
        }

        QDialog::closeEvent(event);
    }
};

#endif // CONTROLLERPAKSELECTIONWINDOW_H
