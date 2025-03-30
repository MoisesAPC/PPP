#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/******************************************************************************
 * MainWindow.h
 *
 * Header file for MainWindow.cpp
 *****************************************************************************/

#include "ui_MainWindow.h"
#include "include/bit.h"
#include "include/save/Save.h"
#include "include/windows/ComboBoxData.h"

#include <QMainWindow>
#include <QSettings>        // QSettings

QT_BEGIN_NAMESPACE
namespace Ui {

class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* instance;

    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // With this struct, we can keep track of the components inside the "Slot" menu option
    struct SlotMenu {
        QMenu* slotOption;
        QAction* mainSaveOption;
        QAction* beginningOfStageSaveOption;
    };

    void populateMainWindow(SaveData* save);

private slots:
    void setupPageMain();
    void setupPageItems();
    void setupPageEventFlags();

    void setupFileMenu();
    void setupSlotMenu();

    void handleNumberOnlyInputUnsigned(std::function<void(unsigned int)> setter, QLineEdit* lineEdit);

    void setupLineEditNumberUnsigned(QLineEdit* lineEdit, const unsigned int minValue, const unsigned int maxValue, std::function<void(unsigned int)> setter);

    void setupComboBox(QComboBox* comboBox, const Ui::ComboBoxData& array, std::function<void(int)> setter);
    void setupComboBoxBitflag(QComboBox* comboBox, const Ui::ComboBoxData& array, unsigned int& variable);
    void onPageButtonClicked(QStackedWidget* stackedWidget, const QWidget* page);
    void switchPage(QStackedWidget* stackedWidgetPages, const QWidget* page);
    void checkMandragoraAndNitroLineEdits();
    QLineEdit* createGridFlag(QGridLayout* gridLayout, int flagSet, unsigned int flags);
    void setupCheckBox(QCheckBox* checkBox, unsigned int value, std::function<void(unsigned int)> setter, std::function<void(unsigned int)> unsetter);
    void fileOpenMenu();
    void fileSaveMenu();
    void fileSaveAsMenu();
    void databaseMenu();
    void updateSlotMenuCheckedState(int selectedSlotIndex, bool isMainSave);

    void openFile(const QString& filename);
    const QString getLastOpenedDirectory(const QSettings& settings);
    void setLastOpenedDirectory(QSettings& settings, const QString filename);

    void setSelectedSave(const int slot) {
        selectedSlot = slot;
    }

    int getSelectedSave() const {
        return selectedSlot;
    }

    void setIsMain(const bool isMain_) {
        isMain = isMain_;
    }

    bool getIsMain() const {
        return isMain;
    }

    void selectComboBoxOption(QComboBox& comboBox, const QVariant data);

    // This function unsets all the comboBoxData values from "variable",
    // and then sets the newValue into the variable
    void updateBitSelection(unsigned int& variable, unsigned int newValue, const Ui::ComboBoxData& comboBoxData) {
        for (const auto& data: comboBoxData) {
            for (const auto& entry: data) {
                BITS_UNSET(variable, entry.second);
            }
        }

        BITS_SET(variable, newValue);
    }

    void enableUIComponents(bool);
    void updateCheckboxEnabledVisibility();
    void updateWindowVisibility(bool);

private:
    Ui::MainWindow* ui;
    QLineEdit* hexBitflagLineEdits[NUM_EVENT_FLAGS] = {};

    int selectedSlot = 0;
    bool isMain = true;

    SlotMenu slotMenuOptions[4] = {};

    Ui::ComboBoxData comboBoxDataMap = {
        {{"Forest of Silence", 0}},
        {{"Castle Wall (Towers)", 1}},
        {{"Castle Wall (Main)", 2}},
        {{"Villa (Yard)", 3}},
        {{"Villa (Foyer)", 4}},
        {{"Villa (Hallway)", 5}},
        {{"Villa (Maze Garden)", 6}},
        {{"Tunnel", 7}},
        {{"Underground Waterway", 8}},
        {{"Castle Center (Main)", 9}},
        {{"Castle Center (Bottom Elevator)", 10}},
        {{"Castle Center (Gears)", 11}},
        {{"Castle Center (Friendly Lizard-man)", 12}},
        {{"Castle Center (Library)", 13}},
        {{"Castle Center (Nitro Room)", 14}},
        {{"Castle Center (Top Elevator)", 15}},
        {{"Tower of Execution", 16}},
        {{"Tower of Sorcery", 17}},
        {{"Tower of Science", 18}},
        {{"Duel Tower", 19}},
        {{"Castle Keep Stairs", 20}},
        {{"Castle Keep", 21}},
        {{"Intro Cutscene Map", 22}},
        {{"Clock Tower", 23}},
        {{"Dracula Desert", 24}},
        {{"Rose / Actrice Fan Room", 25}},
        {{"Villa (Vampire Crypt)", 26}},
        {{"Room of Clocks", 27}},
        {{"Ending Map", 28}},
        {{"Test Grid", 29}}
    };

    Ui::ComboBoxData comboBoxDataCharacter = {
        {{"Reinhardt", 0}},
        {{"Carrie", 1}}
    };

    Ui::ComboBoxData comboBoxDataButtonConfig = {
        {{"Type A", 0}},
        {{"Type B", 1}},
        {{"Type C", 2}}
    };

    Ui::ComboBoxData comboBoxDataSoundMode = {
        {{"Stereo", 0}},
        {{"Monoaural", 1}}
    };

    Ui::ComboBoxData comboBoxDataSubweapon = {
        {{"None", 0}},
        {{"Knife", 1}},
        {{"Holy Water", 2}},
        {{"Cross", 3}},
        {{"Axe", 4}},
        {{"Wooden Stake", 5}},
        {{"Rose", 6}}
    };

    Ui::ComboBoxData comboBoxDataDifficulty = {
        {{"Easy", BIT(4)}},
        {{"Normal", BIT(5)}},
        {{"Hard", BIT(6)}},
    };

    Ui::ComboBoxData comboBoxDataEndingReinhardt = {
        {{"-", 0}},
        {{"Good", BIT(17)}},
        {{"Bad", BIT(18)}}
    };

    Ui::ComboBoxData comboBoxDataEndingCarrie = {
        {{"-", 0}},
        {{"Good", BIT(19)}},
        {{"Bad", BIT(20)}}
    };

    Ui::ComboBoxData comboBoxDataRegion = {
        {{"USA", SaveData::USA}},
        {{"JPN", SaveData::JPN}},
        {{"PAL", SaveData::PAL}}
    };

    Ui::ComboBoxData comboBoxDataLanguage = {
        {{"English", SaveData::ENGLISH}},
        {{"German", SaveData::GERMAN}},
        {{"French", SaveData::FRENCH}}
    };
};
#endif // MAINWINDOW_H
