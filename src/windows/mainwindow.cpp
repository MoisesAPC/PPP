/**
 * @file MainWindow.cpp
 * @brief Main window source code file
 *
 * This file contains the source code for the main window of the application
 * (the first window that opens up when booting the application).
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/windows/main/MainWindow.h"
#include "include/windows/Database/DatabaseMainWindow.h"
#include "include/save/SaveManager.h"
#include "include/file/FileManager.h"

#include <QIntValidator>    // With "QIntValidator", we can validate the contents of an integer (see "handleNumberOnlyInput()")
#include <QtGlobal>         // qBound()
#include <climits>          // SHRT_MAX, UINT_MAX, etc
#include <QMessageBox>      // QMessageBox
#include <QDir>             // QDir
#include <QFileDialog>      // QFileDialog
#include <QSpinBox>         // QSpinBox

// Static instance for this window. We use this to access this window's functions in some parts of the code
MainWindow* MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    instance = this;

    ui->setupUi(this);

    // Avoid being able to maximize the window, as the layout looks worse
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    // Initialize pages
    setupPageMain();
    setupPageItems();
    setupPageEventFlags();

    // Initialize toolbar options
    setupFileMenu();
    setupSlotMenu();
    setupEditMenu();

    // Set default values to the savegame at boot
    SaveManager::getInstance()->clear();
    SaveManager::getInstance()->setRegion(SaveData::USA);
    ui->leItemsSpecial3->setEnabled(false);
    ui->leItemsPoutPourri->setEnabled(true);

    // Ensure that we start in the "Main" page
    switchPage(ui->stackedWidgetPages, ui->pageMain);
    switchPage(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage1);

    // Uncheck the "save enabled checkbox" when opening the program
    enableUIComponents(false);
    ui->cboxEnabled->setChecked(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    instance = nullptr;
}

void MainWindow::setupPageMain() {
    // Initialize line edits
    setupLineEditNumberUnsigned(ui->leLife, 1, 100,
        [](short value) {
            SaveManager::getInstance()->setLife(value);
        }
    );
    ui->leLife->setText(QString::number(100));

    setupLineEditNumberUnsigned(ui->leGold, 0, 99999,
        [](unsigned int value) {
            SaveManager::getInstance()->setGold(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leRedJewels, 0, 99,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_RED_JEWEL, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leSpawn, 0, SHRT_MAX,
        [](short value) {
            SaveManager::getInstance()->setSpawn(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leWhiteJewel, 0, USHRT_MAX,
        [](unsigned char value) {
            SaveManager::getInstance()->setWhiteJewel(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leTimesSaved, 0, UINT_MAX,
        [](unsigned int value) {
            SaveManager::getInstance()->setTimesSaved(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leDeathCount, 0, UINT_MAX,
        [](unsigned int value) {
            SaveManager::getInstance()->setDeathCount(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leGoldRenon, 0, UINT_MAX,
        [](unsigned int value) {
            SaveManager::getInstance()->setGoldRenon(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leHourVamp, 0, 23,
        [](unsigned short value) {
            SaveManager::getInstance()->setHourVamp(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leHealthDepletionRate, 0, SHRT_MAX,
        [](unsigned short value) {
            SaveManager::getInstance()->setHealthDepletionRate(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leWeek, 0, SHRT_MAX,
        [](short value) {
            SaveManager::getInstance()->setWeek(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leDay, 0, 7 - 1,
        [](short value) {
            SaveManager::getInstance()->setDay(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leHour, 0, 24 - 1,
        [](short value) {
            SaveManager::getInstance()->setHour(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leMinutes, 0, 60 - 1,
        [](short value) {
            SaveManager::getInstance()->setMinutes(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leSeconds, 0, 60 - 1,
        [](short value) {
            SaveManager::getInstance()->setSeconds(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leMilliseconds, 0, 600 - 1,
        [](unsigned short value) {
            SaveManager::getInstance()->setMilliseconds(value);
        }
    );

    setupLineEditNumberUnsigned(ui->leFrameCount, 0, UINT_MAX,
        [this](unsigned int value) {
            SaveManager::getInstance()->setFramecount(value);
            convertFrameToTime(SaveManager::getInstance()->getFrameCount(), ui->labelPlaytime);
        }
    );

    // Initialize combo boxes and set their default values
    setupComboBox(ui->cbMap, comboBoxDataMap,
        [](int value) { SaveManager::getInstance()->setMap(value); }
    );

    setupComboBox(ui->cbCharacter, comboBoxDataCharacter,
        [](int value) { SaveManager::getInstance()->setCharacter(value); }
    );

    setupComboBox(ui->cbButtonConfig, comboBoxDataButtonConfig,
        [](int value) { SaveManager::getInstance()->setButtonConfig(value); }
    );

    setupComboBox(ui->cbSoundMode, comboBoxDataSoundMode,
        [](int value) { SaveManager::getInstance()->setSoundMode(value); }
    );

    setupComboBox(ui->cbSubweapon, comboBoxDataSubweapon,
        [](int value) { SaveManager::getInstance()->setSubweapon(value); }
    );

    setupComboBoxBitflag(ui->cbDifficulty, comboBoxDataDifficulty);
    setupComboBoxBitflag(ui->cbReinhardtEnding, comboBoxDataEndingReinhardt);
    setupComboBoxBitflag(ui->cbCarrieEnding, comboBoxDataEndingCarrie);

    setupComboBox(ui->cbRegion, comboBoxDataRegion,
        [this](int value) {
            SaveManager::getInstance()->setRegion(value);

        switch (value) {
            default:
            case SaveData::USA:
                ui->cbLanguage->setEnabled(false);
                ui->cbLanguage->setCurrentIndex(0);
                ui->cbLanguage->setItemText(0, "English");
                ui->leItemsSpecial3->setEnabled(false);
                ui->leItemsPoutPourri->setEnabled(true);
                break;

            case SaveData::JPN:
                ui->cbLanguage->setEnabled(false);
                ui->cbLanguage->setCurrentIndex(0);
                ui->cbLanguage->setItemText(0, "Japanese");
                ui->leItemsSpecial3->setEnabled(true);
                ui->leItemsPoutPourri->setEnabled(false);
                break;

            case SaveData::PAL:
                ui->cbLanguage->setEnabled(true);
                ui->cbLanguage->setCurrentIndex(0);
                ui->cbLanguage->setItemText(0, "English");
                ui->leItemsSpecial3->setEnabled(true);
                ui->leItemsPoutPourri->setEnabled(false);
                break;
            }
        }
    );

    setupComboBox(ui->cbLanguage, comboBoxDataLanguage,
        [](int value) { SaveManager::getInstance()->setLanguage(value); }
    );

    // Initialize cbLanguage to USA values
    ui->cbLanguage->setEnabled(false);
    ui->cbLanguage->setCurrentIndex(0);
    ui->cbLanguage->setCurrentText("English");

    // Initialize pages and the buttons that travel to those pages
    // When each button is pressed, "onPageButtonClicked" will be called passing
    // the desired page by arguments
    connect(ui->buttonMain, &QPushButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackedWidgetPages, ui->pageMain);
    });

    connect(ui->buttonItems, &QPushButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackedWidgetPages, ui->pageItems);
    });

    connect(ui->buttonEventFlags, &QPushButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackedWidgetPages, ui->pageEventFlags);
    });

    // In order to avoid the checkbox from being disabled,
    // we ensure that its parent is MainWindow (since it's never disabled by the "enableUiComponents" function)
    ui->cboxEnabled->setParent(this);
    setupCheckBox(ui->cboxEnabled, SaveData::SAVE_FLAG_ACTIVE,
        [this](unsigned int value) {
            SaveManager::getInstance()->setFlags(value);
            updateWindowVisibility(true);
        },

        [this](unsigned int value) {
            SaveManager::getInstance()->unsetFlags(value);
            updateWindowVisibility(false);
        }
    );

    setupCheckBox(ui->cboxHardMode, SaveData::SAVE_FLAG_HARD_MODE_UNLOCKED,
        [](unsigned int value) { SaveManager::getInstance()->setFlags(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetFlags(value); }
    );

    setupCheckBox(ui->cboxUseAlternateCostume, SaveData::SAVE_FLAG_COSTUME_IS_BEING_USED,
        [](unsigned int value) { SaveManager::getInstance()->setFlags(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetFlags(value); }
    );

    setupCheckBox(ui->cboxReinhardtCostume, SaveData::SAVE_FLAG_HAVE_REINHARDT_ALT_COSTUME,
        [](unsigned int value) { SaveManager::getInstance()->setFlags(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetFlags(value); }
    );

    setupCheckBox(ui->cboxCarrieCostume, SaveData::SAVE_FLAG_HAVE_CARRIE_ALT_COSTUME,
        [](unsigned int value) { SaveManager::getInstance()->setFlags(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetFlags(value); }
    );

    setupCheckBox(ui->cboxNitro, SaveData::SAVE_FLAG_CAN_EXPLODE_ON_JUMPING,
        [](unsigned int value) { SaveManager::getInstance()->setFlags(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetFlags(value); }
    );

    setupCheckBox(ui->cboxVamp, SaveData::PLAYER_FLAG_VAMP,
        [](unsigned int value) { SaveManager::getInstance()->setPlayerStatus(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetPlayerStatus(value); }
    );

    setupCheckBox(ui->cboxPoison, SaveData::PLAYER_FLAG_POISON,
        [](unsigned int value) { SaveManager::getInstance()->setPlayerStatus(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetPlayerStatus(value); }
    );

    setupCheckBox(ui->cboxSto, SaveData::PLAYER_FLAG_STO,
        [](unsigned int value) { SaveManager::getInstance()->setPlayerStatus(value); },
        [](unsigned int value) { SaveManager::getInstance()->unsetPlayerStatus(value); }
    );
}

void MainWindow::setupPageItems() {
    // Jewels
    setupLineEditNumberUnsigned(ui->leItemsSpecial1, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_SPECIAL1, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsSpecial2, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_SPECIAL2, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsSpecial3, 0, 1,
        [](unsigned char value) {
            // This item is version exclusive. Ensure we're only setting it when its associated region is set.
            if (SaveManager::getInstance()->getRegion() == SaveData::PAL ||
                SaveManager::getInstance()->getRegion() == SaveData::JPN) {
                SaveManager::getInstance()->setItem(SaveData::ITEM_ID_SPECIAL3, value);
            }
        }
    );

    // Healing and effect-cure items
    setupLineEditNumberUnsigned(ui->leItemsRoastChicken, 0, 10,
        [](unsigned char value) {
            // In the JPN and PAL versions, this item's ID is the same as USA, but added +1.
            int actualItemId = SaveData::ITEM_ID_ROAST_CHICKEN;
            if (SaveManager::getInstance()->getRegion() == SaveData::PAL ||
                SaveManager::getInstance()->getRegion() == SaveData::JPN) {
                actualItemId++;
            }

            SaveManager::getInstance()->setItem(actualItemId, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsRoastBeef, 0, 10,
        [](unsigned char value) {
            // In the JPN and PAL versions, this item's ID is the same as USA, but added +1.
            int actualItemId = SaveData::ITEM_ID_ROAST_BEEF;
            if (SaveManager::getInstance()->getRegion() == SaveData::PAL ||
                SaveManager::getInstance()->getRegion() == SaveData::JPN) {
                actualItemId++;
            }

            SaveManager::getInstance()->setItem(actualItemId, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsPurifying, 0, 10,
        [](unsigned char value) {
            // In the JPN and PAL versions, this item's ID is the same as USA, but added +1.
            int actualItemId = SaveData::ITEM_ID_PURIFYING;
            if (SaveManager::getInstance()->getRegion() == SaveData::PAL ||
                SaveManager::getInstance()->getRegion() == SaveData::JPN) {
                actualItemId++;
            }

            SaveManager::getInstance()->setItem(actualItemId, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsCureAmpoule, 0, 10,
        [](unsigned char value) {
            // In the JPN and PAL versions, this item's ID is the same as USA, but added +1.
            int actualItemId = SaveData::ITEM_ID_CURE_AMPOULE;
            if (SaveManager::getInstance()->getRegion() == SaveData::PAL ||
                SaveManager::getInstance()->getRegion() == SaveData::JPN) {
                actualItemId++;
            }

            SaveManager::getInstance()->setItem(actualItemId, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsPoutPourri, 0, 10,
        [](unsigned char value) {
            // This item is version exclusive. Ensure we're only setting it when its associated region is set.
            if (SaveManager::getInstance()->getRegion() == SaveData::USA) {
                SaveManager::getInstance()->setItem(SaveData::ITEM_ID_POUT_POURRI, value);
            }
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsHealingKit, 0, 10,
        [](unsigned char value) {
            // In the JPN and PAL versions, this item's ID is the same as USA, but added +1.
            int actualItemId = SaveData::ITEM_ID_HEALING_KIT;
            if (SaveManager::getInstance()->getRegion() == SaveData::PAL ||
                SaveManager::getInstance()->getRegion() == SaveData::JPN) {
                actualItemId++;
            }

            SaveManager::getInstance()->setItem(actualItemId, value);
        }
    );

    // Quest Items
    setupLineEditNumberUnsigned(ui->leItemsSunCard, 0, 10,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_SUN_CARD, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsMoonCard, 0, 10,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_MOON_CARD, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsNitro, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_MAGICAL_NITRO, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsMandragora, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_MANDRAGORA, value);
        }
    );

    // Keys
    setupLineEditNumberUnsigned(ui->leKeyScience1, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_SCIENCE_KEY1, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyScience2, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_SCIENCE_KEY2, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyScience3, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_SCIENCE_KEY3, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyClocktower1, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_CLOCKTOWER_KEY1, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyClocktower2, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_CLOCKTOWER_KEY2, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyClocktower3, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_CLOCKTOWER_KEY3, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyChamber, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_CHAMBER_KEY, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyCopper, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_COPPER_KEY, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyExecution, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_EXECUTION_KEY, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyGarden, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_GARDEN_KEY, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyLeftTower, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_LEFT_TOWER_KEY, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyArchives, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_ARCHIVES_KEY, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leKeyStoreroom, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_STOREROOM_KEY, value);
        }
    );

    // Unused items
    setupLineEditNumberUnsigned(ui->leItemsER, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_ENGAGEMENT_RING, value);
        }
    );

    setupLineEditNumberUnsigned(ui->leItemsIG, 0, 1,
        [](unsigned char value) {
            SaveManager::getInstance()->setItem(SaveData::ITEM_ID_INCANDESCENT_GAZE, value);
        }
    );
}

void MainWindow::setupPageEventFlags() {
    /* Page 1 */
    ui->labelSet0->setText("Set 0 (Forest of Silence, Intro Narration Map, Test Grid)");
    hexBitflagLineEdits[0] = createGridFlag(ui->gridFlagSet0, 0, 0);

    ui->labelSet1->setText("Set 1 (Villa Foyer, Villa Hallway)");
    hexBitflagLineEdits[1] = createGridFlag(ui->gridFlagSet1, 1, 0);

    ui->labelSet2->setText("Set 2 (Underground Waterway, Castle Center - Top elevator room)");
    hexBitflagLineEdits[2] = createGridFlag(ui->gridFlagSet2, 2, 0);

    /* Page 2 */
    ui->labelSet3->setText("Set 3 (Castle Center - Friendly lizard man, Castle Center - Nitro room)");
    hexBitflagLineEdits[3] = createGridFlag(ui->gridFlagSet3, 3, 0);

    ui->labelSet4->setText("Set 4 (Tower of Execution, Tower of Sorcery, Duel Tower)");
    hexBitflagLineEdits[4] = createGridFlag(ui->gridFlagSet4, 4, 0);

    ui->labelSet5->setText("Set 5 (Castle Keep - Stairs, Castle Keep, Clock Tower)");
    hexBitflagLineEdits[5] = createGridFlag(ui->gridFlagSet5, 5, 0);

    /* Page 3 */
    ui->labelSet6->setText("Set 6 (Dracula Desert, Rose / Actriese room, Room of Clocks)");
    hexBitflagLineEdits[6] = createGridFlag(ui->gridFlagSet6, 6, 0);

    ui->labelSet7->setText("Set 7 (Tower of Science - Turrets)");
    hexBitflagLineEdits[7] = createGridFlag(ui->gridFlagSet7, 7, 0);

    ui->labelSet8->setText("Set 8 (Castle Center - Bottom Elevator, Castle Center - Gears room)");
    hexBitflagLineEdits[8] = createGridFlag(ui->gridFlagSet8, 8, 0);

    /* Page 4 */
    ui->labelSet9->setText("Set 9 (Villa - Front Yard)");
    hexBitflagLineEdits[9] = createGridFlag(ui->gridFlagSet9, 9, 0);

    ui->labelSet10->setText("Set 10 (Castle Wall - Main)");
    hexBitflagLineEdits[10] = createGridFlag(ui->gridFlagSet10, 10, 0);

    ui->labelSet11->setText("Set 11 (Maze Garden, Castle Center - Library)");
    hexBitflagLineEdits[11] = createGridFlag(ui->gridFlagSet11, 11, 0);

    /* Page 5 */
    ui->labelSet12->setText("Set 12 (Tunnel)");
    hexBitflagLineEdits[12] = createGridFlag(ui->gridFlagSet12, 12, 0);

    ui->labelSet13->setText("Set 13 (Castle Center - Main)");
    hexBitflagLineEdits[13] = createGridFlag(ui->gridFlagSet13, 13, 0);

    ui->labelSet14->setText("Set 14 (Castle Wall - Towers)");
    hexBitflagLineEdits[14] = createGridFlag(ui->gridFlagSet14, 14, 0);

    /* Page 6 */
    ui->labelSet15->setText("Set 15 (Tower of Science)");
    hexBitflagLineEdits[15] = createGridFlag(ui->gridFlagSet15, 15, 0);


    // Initialize pages and the buttons that travel to those pages
    // When each button is pressed, "onPageButtonClicked" will be called passing
    // the desired page by arguments
    connect(ui->rbEventFlagPage1, &QRadioButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage1);
    });

    connect(ui->rbEventFlagPage2, &QRadioButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage2);
    });

    connect(ui->rbEventFlagPage3, &QRadioButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage3);
    });

    connect(ui->rbEventFlagPage4, &QRadioButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage4);
    });

    connect(ui->rbEventFlagPage5, &QRadioButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage5);
    });

    connect(ui->rbEventFlagPage6, &QRadioButton::clicked, this, [this]() {
        onPageButtonClicked(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage6);
    });
}

/**
 * @brief Given a save data struct, fill all the UI components with the data from the save.
 */
void MainWindow::populateMainWindow(SaveData* saveData) {
    if (saveData == nullptr) {
        return;
    }

    // Combo boxes
    selectComboBoxOption(*ui->cbCharacter, saveData->character);
    selectComboBoxOption(*ui->cbButtonConfig, saveData->button_config);
    selectComboBoxOption(*ui->cbSoundMode, saveData->sound_mode);
    selectComboBoxOption(*ui->cbSubweapon, saveData->subweapon);
    selectComboBoxOption(*ui->cbMap, saveData->map);

    selectComboBoxOption(*ui->cbDifficulty, saveData->getFlag(SaveData::SAVE_FLAG_EASY | SaveData::SAVE_FLAG_NORMAL | SaveData::SAVE_FLAG_HARD));
    selectComboBoxOption(*ui->cbReinhardtEnding, saveData->getFlag(SaveData::SAVE_FLAG_REINDHART_GOOD_ENDING | SaveData::SAVE_FLAG_REINDHART_BAD_ENDING));
    selectComboBoxOption(*ui->cbCarrieEnding, saveData->getFlag(SaveData::SAVE_FLAG_CARRIE_GOOD_ENDING | SaveData::SAVE_FLAG_CARRIE_BAD_ENDING));
    selectComboBoxOption(*ui->cbRegion, SaveManager::getInstance()->getRegion());

    if (SaveManager::getInstance()->getRegion() == SaveData::PAL) {
        selectComboBoxOption(*ui->cbLanguage, saveData->language);
    }

    // Numerical Line edits
    ui->leLife->setText(QString::number(saveData->life));
    ui->leGold->setText(QString::number(saveData->gold));
    ui->leRedJewels->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_RED_JEWEL)));
    ui->leSpawn->setText(QString::number(saveData->spawn));
    ui->leWhiteJewel->setText(QString::number(saveData->save_crystal_number));
    ui->leTimesSaved->setText(QString::number(saveData->time_saved_counter));
    ui->leDeathCount->setText(QString::number(saveData->death_counter));
    ui->leGoldRenon->setText(QString::number(saveData->gold_spent_on_Renon));
    ui->leHourVamp->setText(QString::number(saveData->current_hour_VAMP));
    ui->leHealthDepletionRate->setText(QString::number(saveData->health_depletion_rate_while_poisoned));
    ui->leWeek->setText(QString::number(saveData->week));
    ui->leDay->setText(QString::number(saveData->day));
    ui->leHour->setText(QString::number(saveData->hour));
    ui->leMinutes->setText(QString::number(saveData->minute));
    ui->leSeconds->setText(QString::number(saveData->seconds));
    ui->leMilliseconds->setText(QString::number(saveData->milliseconds));
    ui->leFrameCount->setText(QString::number(saveData->gameplay_framecount));
    convertFrameToTime(saveData->gameplay_framecount, ui->labelPlaytime);

    ui->leItemsSpecial1->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SPECIAL1)));
    ui->leItemsSpecial2->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SPECIAL2)));

    if (SaveManager::getInstance()->getRegion() == SaveData::PAL ||
        SaveManager::getInstance()->getRegion() == SaveData::JPN) {
        ui->leItemsSpecial3->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SPECIAL3)));
    }
    else {
        ui->leItemsSpecial3->setText("0");
    }

    if (SaveManager::getInstance()->getRegion() == SaveData::USA) {
        ui->leItemsRoastChicken->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ROAST_CHICKEN)));
        ui->leItemsRoastBeef->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ROAST_BEEF)));
        ui->leItemsHealingKit->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_HEALING_KIT)));
        ui->leItemsPurifying->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_PURIFYING)));
        ui->leItemsCureAmpoule->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_CURE_AMPOULE)));
    }
    else {
        ui->leItemsRoastChicken->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ROAST_CHICKEN + 1)));
        ui->leItemsRoastBeef->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ROAST_BEEF + 1)));
        ui->leItemsHealingKit->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_HEALING_KIT + 1)));
        ui->leItemsPurifying->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_PURIFYING + 1)));
        ui->leItemsCureAmpoule->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_CURE_AMPOULE + 1)));
    }

    if (SaveManager::getInstance()->getRegion() == SaveData::USA) {
        ui->leItemsPoutPourri->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_POUT_POURRI)));
    }
    else {
        ui->leItemsPoutPourri->setText("0");
    }

    ui->leItemsSunCard->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SUN_CARD)));
    ui->leItemsMoonCard->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_MOON_CARD)));
    ui->leItemsNitro->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_MAGICAL_NITRO)));
    ui->leItemsMandragora->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_MANDRAGORA)));
    ui->leKeyArchives->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ARCHIVES_KEY)));
    ui->leKeyLeftTower->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_LEFT_TOWER_KEY)));
    ui->leKeyStoreroom->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_STOREROOM_KEY)));
    ui->leKeyGarden->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_GARDEN_KEY)));
    ui->leKeyCopper->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_COPPER_KEY)));
    ui->leKeyChamber->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_CHAMBER_KEY)));
    ui->leKeyExecution->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_EXECUTION_KEY)));
    ui->leKeyScience1->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SCIENCE_KEY1)));
    ui->leKeyScience2->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SCIENCE_KEY2)));
    ui->leKeyScience3->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SCIENCE_KEY3)));
    ui->leKeyClocktower1->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_CLOCKTOWER_KEY1)));
    ui->leKeyClocktower2->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_CLOCKTOWER_KEY2)));
    ui->leKeyClocktower3->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_CLOCKTOWER_KEY3)));
    ui->leItemsER->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ENGAGEMENT_RING)));
    ui->leItemsIG->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_INCANDESCENT_GAZE)));

    // Checkboxes
    ui->cboxEnabled->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_ACTIVE));
    updateCheckboxEnabledVisibility();

    ui->cboxHardMode->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_HARD_MODE_UNLOCKED));
    ui->cboxUseAlternateCostume->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_COSTUME_IS_BEING_USED));
    ui->cboxReinhardtCostume->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_HAVE_REINHARDT_ALT_COSTUME));
    ui->cboxCarrieCostume->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_HAVE_CARRIE_ALT_COSTUME));
    ui->cboxNitro->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_CAN_EXPLODE_ON_JUMPING));
    ui->cboxVamp->setChecked(saveData->getPlayerStatus(SaveData::PLAYER_FLAG_VAMP));
    ui->cboxPoison->setChecked(saveData->getPlayerStatus(SaveData::PLAYER_FLAG_POISON));
    ui->cboxSto->setChecked(saveData->getPlayerStatus(SaveData::PLAYER_FLAG_STO));

    // Event flag grids. We edit each of the line edits associated to the event flags to assign the hex value gotten
    // from the save data. Then, the checkboxes will be ticked / unticked automatically
    for (unsigned int i = 0; i < NUM_EVENT_FLAGS; i++) {
        hexBitflagLineEdits[i]->setText(QString::number(saveData->getEventFlags(i)));
    }
}

/**
 * When we press on a checkbox, a setter function will be called that will set a certain value.
 * Likewise, when we uncheck them, an unsetter function will be called which will remove the value setted in the setter.
 */
void MainWindow::setupCheckBox(QCheckBox* checkBox, unsigned int value, std::function<void(unsigned int)> setter, std::function<void(unsigned int)> unsetter) {
    connect(checkBox, &QCheckBox::toggled, [setter, unsetter, value](bool checked) {
        if (checked) {
            setter(value);
        }
        else {
            unsetter(value);
        }
    });
}

void MainWindow::openFile(const QString& filename) {
    int result = FileManager::getInstance()->openFile(filename);

    if (result == -1) {
        QMessageBox::critical(this, "Error", "Couldn't open file.");
        return;
    }

    // Populate with the currently-selected save slot.
    populateMainWindow(&SaveManager::getInstance()->getCurrentSave());

    /// Set default Slot option to the currently selected save.
    /// @note We must call this function after calling "populateMainWindow" in order to have the checkboxes
    /// ready. Otherwise the program will throw SIGSEV.
    updateSlotMenuCheckedState(SaveManager::getInstance()->currentSave, SaveManager::getInstance()->isMain);
}

void MainWindow::fileOpenMenu() {
    // With these settings, we can obtain the last opened save directory
    QSettings settings("PPP", "Castlevania 64 Save Editor");

    // Retrieve the last opened file path
    QString lastOpenedDir = settings.value("lastOpenedDir", QDir::homePath()).toString();

    // Open file menu in the last opened directory by default
    QString filename = QFileDialog::getOpenFileName(
        this, "Open File", lastOpenedDir,
        "All accepted filetypes (*.mpk *.pak *.note *.eep *.n64 *.t64);;"
        "Individual note (*.note);;"
        "Controller Pak data (*.mpk *.pak);;"
        "Cartridge (Japanese version only) (*.eep);;"
        "DexDrive saves (*.n64 *.t64);;"
        "All Files (*)"
    );

    if (!filename.isEmpty()) {
        openFile(filename);

        /// Store the directory of the opened file
        /// @note If on Linux, the settings are saved at
        /// ~/.config/PPP/Castlevania 64 Save Editor.conf
        /// In Windows, the dir is stored at this registry:
        /// HKEY_CURRENT_USER\Software\PPP\Castlevania 64 Save Editor
        QFileInfo fileInfo(filename);
        settings.setValue("lastOpenedDir", fileInfo.absolutePath());
    }
}

void MainWindow::fileSaveMenu() {
    if (!FileManager::getInstance()->wasFileOpened()) {
        QMessageBox::critical(this, "Error", "Open a file in local before trying to save.");
        return;
    }

    /// @note Scrapping this since we need to save empty saves in order to save New Game+ files.
    /*
    if (SaveManager::getInstance()->areAllSavesDisabled()) {
        QMessageBox::critical(this, "Error", "This save is empty, so it can't be saved.");
        return;
    }
*/

    int result = FileManager::getInstance()->writeFile(FileManager::getInstance()->getFilepath(), false);

    switch (result) {
        case -1:
            QMessageBox::critical(this, "Error", "File could not be saved.\n"
                                                 "Make sure to save it as one of the accepted filetypes.");
            return;

        case -2:
            QMessageBox::critical(this, "Error", "Make sure that at least one save is enabled before saving.\n");
            return;
    }

    QMessageBox::information(this, "Save", "Saved successfully");
}

void MainWindow::fileSaveAsMenu() {
    /// @note Scrapping this since we need to save empty saves in order to save New Game+ files.
    /*
    if (SaveManager::getInstance()->areAllSavesDisabled()) {
        QMessageBox::critical(this, "Error", "This save is empty, so it can't be saved.");
        return;
    }
*/

    // Retrieve the last "Save As..." directory
    QSettings settings("PPP", "Castlevania 64 Save Editor");
    QString lastSaveAsDirectory = settings.value("lastSaveDir", QDir::homePath()).toString();

    QString filepath = QFileDialog::getSaveFileName(
        this, "Save As...", lastSaveAsDirectory,
        "Individual note (*.note);;"
        "Cartridge (Japanese version only) (*.eep)"
    );

    if (!filepath.isEmpty()) {
        int result = FileManager::getInstance()->writeFile(filepath, true);

        switch (result) {
            case -1:
                QMessageBox::critical(this, "Error", "File could not be saved.\n"
                                                     "Make sure to save it as one of the accepted filetypes.");
                return;

            case -2:
                QMessageBox::critical(this, "Error", "Make sure that at least one save is enabled before saving.\n");
                return;
        }
    }
    else {
        return;
    }

    /// Store the directory of the saved file
    /// @note If on Linux, the settings are saved at
    /// ~/.config/PPP/Castlevania 64 Save Editor.conf
    /// In Windows, the dir is stored at this registry:
    /// HKEY_CURRENT_USER\Software\PPP\Castlevania 64 Save Editor
    QFileInfo fileInfo(filepath);
    settings.setValue("lastSaveDir", fileInfo.absolutePath());
    QMessageBox::information(this, "Save", "Saved successfully");
}

void MainWindow::setupFileMenu() {
    // Setup the "Open" button
    connect(ui->actionOpenFile, &QAction::triggered, this, &MainWindow::fileOpenMenu);

    // Setup the "Save" button
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::fileSaveMenu);

    // Setup the "Save As..." button
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::fileSaveAsMenu);

    // Setup the "Exit" button
    connect(ui->actionExit, &QAction::triggered, this, []() {
        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Exit", "Are you sure you want to quit?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            QApplication::exit();
        }
    });

    // Setup the "Database" button
    connect(ui->actionDatabase, &QAction::triggered, this, &MainWindow::databaseMenu);
}

void MainWindow::setupSlotMenu() {
    QMenu* menuSlot = menuBar()->addMenu("Slot");
    menuSlot->setObjectName("Slot");

    for (int i = 0; i < NUM_SAVES; ++i) {
        // Create a submenu for "Slot X"
        slotMenuOptions[i].slotOption = new QMenu(QString("Slot %1").arg(i + 1), this);
        menuSlot->addMenu(slotMenuOptions[i].slotOption);
        slotMenuOptions[i].slotOption->setObjectName(QString("Slot %1").arg(i + 1));

        // Create "Main" action inside the Slot X menu
        slotMenuOptions[i].mainSaveOption = new QAction("Main", this);
        slotMenuOptions[i].mainSaveOption->setCheckable(true);
        slotMenuOptions[i].slotOption->addAction(slotMenuOptions[i].mainSaveOption);
        connect(slotMenuOptions[i].mainSaveOption, &QAction::triggered, this, [this, i]() {
            selectedSlot = i;
            isMain = true;
            updateSlotMenuCheckedState(i, true);
            populateMainWindow(&SaveManager::getInstance()->getSaveSlot(i).mainSave);
            updateWindowVisibility(BITS_HAS(SaveManager::getInstance()->getCurrentSaveSlot().mainSave.flags, SaveData::SAVE_FLAG_ACTIVE));
        });

        // Create "Beginning of Stage" action inside the Slot X menu
        slotMenuOptions[i].beginningOfStageSaveOption = new QAction("Beginning of Stage", this);
        slotMenuOptions[i].beginningOfStageSaveOption->setCheckable(true);
        slotMenuOptions[i].slotOption->addAction(slotMenuOptions[i].beginningOfStageSaveOption);
        connect(slotMenuOptions[i].beginningOfStageSaveOption, &QAction::triggered, this, [this, i]() {
            selectedSlot = i;
            isMain = false;
            updateSlotMenuCheckedState(i, false);
            populateMainWindow(&SaveManager::getInstance()->getSaveSlot(i).beginningOfStage);
            /// @note For enabling / disabling the interface, we only look at the active flag from "mainSave", NOT the one in "beginningOfStage"
            updateWindowVisibility(BITS_HAS(SaveManager::getInstance()->getCurrentSaveSlot().mainSave.flags, SaveData::SAVE_FLAG_ACTIVE));
        });
    }

    /// Set default Slot option to Slot 1 -> Main.
    /// @note We must call this function after calling "populateMainWindow" in order to have the checkboxes
    /// ready. Otherwise the program will throw SIGSEV.
    updateSlotMenuCheckedState(0, true);
}

/// When clicking on a slot option, check it, and *also uncheck* any other unselected options
/// This is done for the "Main" and "Beginning of Stage" slot menu options
void MainWindow::updateSlotMenuCheckedState(int selectedSlotIndex, bool isMainSave) {
    SaveManager* saveManager = SaveManager::getInstance();

    for (unsigned int i = 0; i < NUM_SAVES; ++i) {
        slotMenuOptions[i].mainSaveOption->setChecked(i == selectedSlotIndex && isMainSave);
        slotMenuOptions[i].beginningOfStageSaveOption->setChecked(i == selectedSlotIndex && !isMainSave);
    }

    saveManager->isMain = isMainSave;
    saveManager->currentSave = selectedSlotIndex;
}

void MainWindow::setupEditMenu() {
    connect(ui->actionCopy, &QAction::triggered, this, [this]() {
        onCopy(this);
    });
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDelete);
    connect(ui->actionDelete_All, &QAction::triggered, this, &MainWindow::onDeleteAll);
}

/**
 * @brief Allows both decimal and hexadecimal numbers to be innputted.
 */
void MainWindow::handleNumberOnlyInputUnsigned(std::function<void(unsigned int)> setter, QLineEdit* lineEdit) {
    if (!lineEdit) {
        return;
    }

    QString text = lineEdit->text();
    bool ok = false;
    unsigned int value = 0;

    if (text.startsWith("0x", Qt::CaseInsensitive)) {
        value = text.mid(2).toUInt(&ok, 16);
    }
    else {
        value = text.toUInt(&ok, 10);
    }

    if (ok) {
        const unsigned int minValue = lineEdit->property("minValue").toUInt();
        const unsigned int maxValue = lineEdit->property("maxValue").toUInt();

        value = qBound<unsigned int>(minValue, value, maxValue);

        // We call this function just before setting the text
        checkMandragoraAndNitroLineEdits();

        lineEdit->setText(QString::number(value));

        setter(value);
    }
}

/// With this function, we can have more control during lineEdit initialization.
/// In this case, we make it so that we can add a min and max value to each lineEdit without much copy-pasting
void MainWindow::setupLineEditNumberUnsigned(QLineEdit* lineEdit, const unsigned int minValue, const unsigned int maxValue, std::function<void(unsigned int)> setter) {
    // Using this regex, we can accept either only hex values (preceded "0x") or decimal values
    // of up to 8 digits long (to prevent them from overflowing the max int / uint)
    QRegularExpression acceptDecimalAndHexRegex(R"(^(\d{1,8}|0[xX][0-9A-Fa-f]{1,8})$)");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(acceptDecimalAndHexRegex, this);
    lineEdit->setValidator(validator);

    lineEdit->setProperty("minValue", minValue);
    lineEdit->setProperty("maxValue", maxValue);

    connect(lineEdit, &QLineEdit::editingFinished, this, [this, setter, lineEdit]() {
        handleNumberOnlyInputUnsigned(setter, lineEdit);
    });
}

/// Populate a Combo box given an array of name strings and their associated numeric value
void MainWindow::setupComboBox(QComboBox* comboBox, const Ui::ComboBoxData& array, std::function<void(int)> setter) {
    comboBox->clear();

    for (const auto& map: array) {
        for (const auto& entry: map) {
            comboBox->addItem(QString::fromStdString(entry.first), QVariant(entry.second));
        }
    }

    connect(comboBox, &QComboBox::currentIndexChanged, [comboBox, setter](int index) {
        if (index >= 0) {
            int value = comboBox->itemData(index).toInt();
            setter(value);
        }
    });

    comboBox->setCurrentIndex(0);
}

/// Same as "setupComboBox", but this is meant for comboboxes which are supposed to assign bitflags to a value instead,
/// such as the combo box for setting the game's difficulty
void MainWindow::setupComboBoxBitflag(QComboBox* comboBox, const Ui::ComboBoxData& array) {
    comboBox->clear();

    for (const auto& map: array) {
        for (const auto& entry: map) {
            comboBox->addItem(QString::fromStdString(entry.first), QVariant(entry.second));
        }
    }

    connect(comboBox, &QComboBox::currentIndexChanged, [this, comboBox, array](int) {
        this->handleComboBoxSelection(comboBox, array);
    });

    comboBox->setCurrentIndex(0);
    handleComboBoxSelection(comboBox, array);
}

void MainWindow::updateBitSelection(unsigned int newValue, const Ui::ComboBoxData& comboBoxData) {
    for (const auto& data: comboBoxData) {
        for (const auto& entry: data) {
            SaveManager::getInstance()->unsetFlags(entry.second);
        }
    }

    SaveManager::getInstance()->setFlags(newValue);
}

void MainWindow::handleComboBoxSelection(QComboBox* comboBox, const Ui::ComboBoxData& array) {
    int index = comboBox->currentIndex();
    if (index >= 0) {
        int value = comboBox->itemData(index).toInt();
        this->updateBitSelection(value, array);
    }
}

void MainWindow::selectComboBoxOption(QComboBox& comboBox, const QVariant data) {
    // Workaround to fix a bug where some comboboxes wouldn't update properly when setting
    // an option whose value was 0.
    if (data.toInt() == 0) {
        comboBox.setCurrentIndex(0);
        return;
    }

    int index = comboBox.findData(data);

    if (index != -1) {
        comboBox.setCurrentIndex(index);
    }
}

void MainWindow::onPageButtonClicked(QStackedWidget* stackedWidget, const QWidget* page) {
    switchPage(stackedWidget, page);
}

void MainWindow::switchPage(QStackedWidget* stackedWidget, const QWidget* page) {
    stackedWidget->setCurrentIndex(stackedWidget->indexOf(page));
}

/// The player can only have a Mandragora *OR* a Magical Nitro at the same time.
/// This function checks if both Mandragora and Nitro have an amount larger than 0 at the same time.
/// If so, set the amount for both items back to 0
void MainWindow::checkMandragoraAndNitroLineEdits() {
    QLineEdit* mandragoraLineEdit = ui->leItemsMandragora;
    QLineEdit* nitroLineEdit = ui->leItemsNitro;

    bool okMandragora = false;
    bool okNitro = false;

    if ((mandragoraLineEdit->text().toUInt(&okMandragora, 10) != 0) && (nitroLineEdit->text().toUInt(&okNitro, 10) != 0)) {
        mandragoraLineEdit->setText(QString::number(0));
        nitroLineEdit->setText(QString::number(0));
    }
}

/**
 * @brief Creates the event flag grid dynamically.
 */
QLineEdit* MainWindow::createGridFlag(QGridLayout* gridLayout, int flagSet, unsigned int flags) {
    QLineEdit* hexBitflagDisplay = new QLineEdit();
    hexBitflagDisplay->setAlignment(Qt::AlignRight);
    hexBitflagDisplay->setText(QString("%1").arg(flags, 1, 10, QChar('0')));

    /// Ensure that we're putting "hexBitflagDisplay" in the right most part of the checkboxes, on the 1st row, 8th column
    /// @note We also add an extra row and column for printing the column / row number
    gridLayout->addWidget(hexBitflagDisplay, 0, 9, 5, 1);
    // Add numbers on top and to the right showing the column / row number respectively
    for (unsigned int i = 0; i < 8; ++i) {
        QLabel* colLabel = new QLabel(QString::number(7 - i));
        colLabel->setAlignment(Qt::AlignCenter);  // Center the row numbers
        gridLayout->addWidget(colLabel, 0, i + 1);
    }

    for (unsigned int i = 0; i < 4; ++i) {
        QLabel* rowLabel = new QLabel(QString::number(i));
        rowLabel->setAlignment(Qt::AlignCenter);  // Center the row numbers
        gridLayout->addWidget(rowLabel, 4 - i, 0);
    }

    QVector<QCheckBox*> checkBoxes(32);

    // Create 32 checkboxes (one for each bit)
    for (unsigned int i = 0; i < 32; ++i) {
        QCheckBox* checkBox = new QCheckBox();
        // Set the checked state based on the flag value
        checkBox->setChecked(flags & (1 << i));
        // Add the checkbox to the grid layout. Then place it in a 4x8 grid
        // We add +1 so that we don't print the checkboxes in the same row / column as the numbers
        int row = 3 - (i / 8);
        int col = 8 - (i % 8);
        gridLayout->addWidget(checkBox, row + 1, col);

        checkBoxes[i] = checkBox;
    }

    // Since now each checkbox is initialized, we can go ahead and connect them + add the function to handle them when toggled
    for (unsigned int j = 0; j < 32; j++) {
        // Make sure that each checkbox is updated *on-the-fly* as we edit the hex value from the line edit
        // To do so, we directly define a lambda function that does this for us. This is executed when pressing any of the checkboxes
        connect(checkBoxes[j], &QCheckBox::toggled, this, [checkBoxes, hexBitflagDisplay]() {
            unsigned int updatedFlags = 0;
            for (int m = 0; m < 32; ++m) {
                if (checkBoxes[m]->isChecked()) {
                    updatedFlags |= (1 << m);
                }
            }
            hexBitflagDisplay->setText(QString("%1").arg(updatedFlags, 1, 10, QChar('0')));
        });
    }

    // Lastly, we connect the "hexBitflagDisplay" and add a handling function that will update all
    // checkboxes depending on the hex bitflag value passed in the "hexBitflagDisplay" line edit
    connect(hexBitflagDisplay, &QLineEdit::textChanged, this, [checkBoxes, hexBitflagDisplay, flagSet](const QString& text) {
        bool ok = false;
        unsigned int newFlags = text.toUInt(&ok, 10);

        SaveManager::getInstance()->setEventFlags(flagSet, newFlags);

        // Ensure we limit the input value up to 0xFFFFFFFF
        if (ok && newFlags <= 0xFFFFFFFF) {
            for (int k = 0; k < 32; ++k) {
                checkBoxes[k]->setChecked(newFlags & (1 << k));
            }
        }
        else {
            hexBitflagDisplay->setText("0");
        }
    });

    // We return "hexBitflagDisplay" so that we can access to it later for changing the event flag values by editing the lineEdit
    return hexBitflagDisplay;
}

void MainWindow::enableUIComponents(bool enable) {
    // This function is now scrapped as it sometimes is necessary to edit a disabled save
    // (fo example, to activate New Game+)
    return;
/*
    QList<QWidget*> widgets = this->findChildren<QWidget*>();
    QMenu* menuSlot = menuBar()->findChild<QMenu*>("Slot");

    // Iterate over menuSlot's actions to find "Slot X" submenus.
    // We put these on a list so that later we can just enable them all at once (see the next loop)
    QList<QMenu*> slotSubMenus;
    for (QAction* action: menuSlot->actions()) {
        if (QMenu* submenu = action->menu()) {
            slotSubMenus.append(submenu);
        }
    }

    for (QWidget* widget: widgets) {
        // Skip essential components.
        // Also ensure that the language doesn't get enabled the first time a USA or JPN save is loaded
        if (widget == ui->cboxEnabled || widget == ui->menuBar || widget == ui->menuFile ||
            widget == menuSlot || slotSubMenus.contains(qobject_cast<QMenu*>(widget)) ||
            widget == ui->cbLanguage) {
            continue;
        }

        widget->setEnabled(enable);
    }
*/
}

/// This function ensures that the "Enabled" checkbox is only visible for "Main" saves
void MainWindow::updateCheckboxEnabledVisibility() {
    if (!isMain) {
        ui->cboxEnabled->hide();
    }
    else {
        ui->cboxEnabled->show();
    }
}

/// Make sure to always have the "Beginning of Stage" save enabled only if the "Main" save is enabled
void MainWindow::updateWindowVisibility(bool enable) {
    if (BITS_HAS(SaveManager::getInstance()->getCurrentSaveSlot().mainSave.flags, SaveData::SAVE_FLAG_ACTIVE)
        && !isMain) {
        enableUIComponents(true);
    }
    else {
        enableUIComponents(enable);
    }
}

void MainWindow::databaseMenu() {
    DatabaseMainWindow* databaseAccessWindow = new DatabaseMainWindow(this);
    databaseAccessWindow->exec();
    databaseAccessWindow->close();
}

/// Given a framecount (in 30fps), converts it from frames to hours, minutes and seconds
void MainWindow::convertFrameToTime(const unsigned int frameCount, QLabel* output) {
    int totalSeconds = frameCount / 30;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    output->setText(QString("%1:%2:%3").arg(hours, 2, 10, QChar('0'))
                        .arg(minutes, 2, 10, QChar('0'))
                        .arg(seconds, 2, 10, QChar('0')));
}

/**
 * @brief Copy the currently-opened save slot to another slot.
 */
void MainWindow::onCopy(QWidget* parent) {
    SaveManager* saveManager = SaveManager::getInstance();
    // The save slot where the data will be copied over
    unsigned int destSaveSlot = 0;

    // Dynamically create the window
    QDialog dialog(parent);
    dialog.setWindowTitle("Copy");
    QVBoxLayout layout(&dialog);
    QLabel label("Select a save:", &dialog);
    layout.addWidget(&label);

    // Create the save slot ID selection spinbox
    QSpinBox spinBox(&dialog);
    spinBox.setRange(1, 4);
    layout.addWidget(&spinBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout.addWidget(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        destSaveSlot = spinBox.value() - 1;

        // If trying to copy the current save slot into an slot marked as "Enabled", then ask first if the user
        // wants to overwrite the data.
        if (BITS_HAS(saveManager->getSaveSlot(destSaveSlot).mainSave.flags, SaveData::SAVE_FLAG_ACTIVE)) {
            QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Clear All", "An existing save exists on the destination slot\n"
                                                                                            "Do you want to replace it?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::No) {
                return;
            }
        }

        saveManager->setSaveSlot(saveManager->getCurrentSaveSlot(), destSaveSlot);
        populateMainWindow(&SaveManager::getInstance()->getCurrentSave());
    }
}

/**
 * @brief Clear a the currently opened save slot.
 */
void MainWindow::onDelete() {
    QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Clear", "Are you sure you want to clear the current save?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        SaveManager::getInstance()->getCurrentSaveSlot().clear();
        populateMainWindow(&SaveManager::getInstance()->getCurrentSave());
        updateWindowVisibility(BITS_HAS(SaveManager::getInstance()->getCurrentSaveSlot().mainSave.flags, SaveData::SAVE_FLAG_ACTIVE));
    }
}

/**
 * @brief Clear all save slots within the current save file.
 */
void MainWindow::onDeleteAll() {
    QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Clear All", "Are you sure you want to clear all saves?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        for (int i = 0; i < NUM_SAVES; i++) {
            SaveManager::getInstance()->getSaveSlot(i).clear();
            populateMainWindow(&SaveManager::getInstance()->getCurrentSave());
            updateWindowVisibility(BITS_HAS(SaveManager::getInstance()->getCurrentSaveSlot().mainSave.flags, SaveData::SAVE_FLAG_ACTIVE));
        }
    }
}
