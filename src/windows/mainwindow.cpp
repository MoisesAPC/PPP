#include "include\windows\main\mainwindow.h"
#include "include\save\SaveManager.h"

#include <QIntValidator>    // With "QIntValidator", we can validate the contents of an integer (see "handleNumberOnlyInput()")
#include <QtGlobal>         // qBound()
#include <climits>          // SHRT_MAX, UINT_MAX, etc
#include <QMessageBox>      // QMessageBox
#include <QDir>             // QDir
#include <QFileDialog>      // QFileDialog

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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

    // Ensure that we start in the "Main" page
    switchPage(ui->stackedWidgetPages, ui->pageMain);
    switchPage(ui->stackWidgetEventFlagsPages, ui->EventFlagsPage1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPageMain() {
    // Initialize line edits
    setupLineEditNumberUnsigned(ui->leLife, 1, 100);
    setupLineEditNumberUnsigned(ui->leGold, 0, 99999);
    setupLineEditNumberUnsigned(ui->leRedJewels, 0, 99);

    setupLineEditNumberUnsigned(ui->leSpawn, 0, SHRT_MAX);
    setupLineEditNumberUnsigned(ui->leWhiteJewel, 0, UCHAR_MAX);

    setupLineEditNumberUnsigned(ui->leTimesSaved, 0, UINT_MAX);
    setupLineEditNumberUnsigned(ui->leDeathCount, 0, UINT_MAX);
    setupLineEditNumberUnsigned(ui->leGoldRenon, 0, UINT_MAX);
    setupLineEditNumberUnsigned(ui->leHourVamp, 0, 23);
    setupLineEditNumberUnsigned(ui->leHealthDepletionRate, 0, SHRT_MAX);

    setupLineEditNumberUnsigned(ui->leWeek, 0, SHRT_MAX);
    setupLineEditNumberUnsigned(ui->leDay, 0, 7 - 1);
    setupLineEditNumberUnsigned(ui->leHour, 0, 24 - 1);
    setupLineEditNumberUnsigned(ui->leMinutes, 0, 60 - 1);
    setupLineEditNumberUnsigned(ui->leSeconds, 0, 60 - 1);
    setupLineEditNumberUnsigned(ui->leMilliseconds, 0, 600 - 1);
    setupLineEditNumberUnsigned(ui->leFrameCount, 0, UINT_MAX);

    // Initialize combo boxes and set their default values
    setupComboBox(ui->cbMap, comboBoxDataMap);
    ui->cbMap->setCurrentIndex(0);

    setupComboBox(ui->cbCharacter, comboBoxDataCharacter);
    ui->cbCharacter->setCurrentIndex(0);

    setupComboBox(ui->cbButtonConfig, comboBoxDataButtonConfig);
    ui->cbButtonConfig->setCurrentIndex(0);

    setupComboBox(ui->cbSoundMode, comboBoxDataSoundMode);
    ui->cbSoundMode->setCurrentIndex(0);

    setupComboBox(ui->cbSubweapon, comboBoxDataSubweapon);
    ui->cbSubweapon->setCurrentIndex(0);

    setupComboBox(ui->cbDifficulty, comboBoxDataDifficulty);
    ui->cbDifficulty->setCurrentIndex(0);

    setupComboBox(ui->cbReinhardtEnding, comboBoxDataEndingReinhardt);
    ui->cbReinhardtEnding->setCurrentIndex(0);

    setupComboBox(ui->cbCarrieEnding, comboBoxDataEndingCarrie);
    ui->cbCarrieEnding->setCurrentIndex(0);

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
}

void MainWindow::setupPageItems() {
    // Jewels
    setupLineEditNumberUnsigned(ui->leItemsSpecial1, 0, 1);
    setupLineEditNumberUnsigned(ui->leItemsSpecial2, 0, 1);

    // Healing and effect-cure items
    setupLineEditNumberUnsigned(ui->leItemsRoastChicken, 0, 10);
    setupLineEditNumberUnsigned(ui->leItemsRoastBeef, 0, 10);
    setupLineEditNumberUnsigned(ui->leItemsHealingKit, 0, 10);
    setupLineEditNumberUnsigned(ui->leItemsPurifying, 0, 10);
    setupLineEditNumberUnsigned(ui->leItemsCureAmpoule, 0, 10);
    setupLineEditNumberUnsigned(ui->leItemsPoutPourri, 0, 10);

    // Quest Items
    setupLineEditNumberUnsigned(ui->leItemsSunCard, 0, 10);
    setupLineEditNumberUnsigned(ui->leItemsMoonCard, 0, 10);
    setupLineEditNumberUnsigned(ui->leItemsNitro, 0, 1);
    setupLineEditNumberUnsigned(ui->leItemsMandragora, 0, 1);

    // Keys
    setupLineEditNumberUnsigned(ui->leKeyScience1, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyScience2, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyScience3, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyClocktower1, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyClocktower2, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyClocktower3, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyChamber, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyCopper, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyExecution, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyGarden, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyLeftTower, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyArchives, 0, 1);
    setupLineEditNumberUnsigned(ui->leKeyStoreroom, 0, 1);

    // Unused items
    setupLineEditNumberUnsigned(ui->leItemsER, 0, 1);
    setupLineEditNumberUnsigned(ui->leItemsIG, 0, 1);
}

void MainWindow::setupPageEventFlags() {
    /* Page 1 */
    ui->labelSet0->setText("Set 0 (Forest of Silence, Intro Narration Map, Test Grid)");
    hexBitflagLineEdits[0] = createGridFlag(ui->gridFlagSet0, 0);

    ui->labelSet1->setText("Set 1 (Villa Foyer, Villa Hallway)");
    hexBitflagLineEdits[1] = createGridFlag(ui->gridFlagSet1, 0);

    ui->labelSet2->setText("Set 2 (Underground Waterway, Castle Center - Top elevator room)");
    hexBitflagLineEdits[2] = createGridFlag(ui->gridFlagSet2, 0);

    /* Page 2 */
    ui->labelSet3->setText("Set 3 (Castle Center - Friendly lizard man, Castle Center - Nitro room)");
    hexBitflagLineEdits[3] = createGridFlag(ui->gridFlagSet3, 0);

    ui->labelSet4->setText("Set 4 (Tower of Execution, Tower of Sorcery, Duel Tower)");
    hexBitflagLineEdits[4] = createGridFlag(ui->gridFlagSet4, 0);

    ui->labelSet5->setText("Set 5 (Castle Keep - Stairs, Castle Keep, Clock Tower)");
    hexBitflagLineEdits[5] = createGridFlag(ui->gridFlagSet5, 0);

    /* Page 3 */
    ui->labelSet6->setText("Set 6 (Dracula Desert, Rose / Actriese room, Room of Clocks)");
    hexBitflagLineEdits[6] = createGridFlag(ui->gridFlagSet6, 0);

    ui->labelSet7->setText("Set 7 (Tower of Science - Turrets)");
    hexBitflagLineEdits[7] = createGridFlag(ui->gridFlagSet7, 0);

    ui->labelSet8->setText("Set 8 (Castle Center - Bottom Elevator, Castle Center - Gears room)");
    hexBitflagLineEdits[8] = createGridFlag(ui->gridFlagSet8, 0);

    /* Page 4 */
    ui->labelSet9->setText("Set 9 (Villa - Front Yard)");
    hexBitflagLineEdits[9] = createGridFlag(ui->gridFlagSet9, 0);

    ui->labelSet10->setText("Set 10 (Castle Wall - Main)");
    hexBitflagLineEdits[10] = createGridFlag(ui->gridFlagSet10, 0);

    ui->labelSet11->setText("Set 11 (Maze Garden, Castle Center - Library)");
    hexBitflagLineEdits[11] = createGridFlag(ui->gridFlagSet11, 0);

    /* Page 5 */
    ui->labelSet12->setText("Set 12 (Tunnel)");
    hexBitflagLineEdits[12] = createGridFlag(ui->gridFlagSet12, 0);

    ui->labelSet13->setText("Set 13 (Castle Center - Main)");
    hexBitflagLineEdits[13] = createGridFlag(ui->gridFlagSet13, 0);

    ui->labelSet14->setText("Set 14 (Castle Wall - Towers)");
    hexBitflagLineEdits[14] = createGridFlag(ui->gridFlagSet14, 0);

    /* Page 6 */
    ui->labelSet15->setText("Set 15 (Tower of Science)");
    hexBitflagLineEdits[15] = createGridFlag(ui->gridFlagSet15, 0);


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

// Get the setting to load the last opened directory (defaulting to the current dir if none is found)
const QString MainWindow::getLastOpenedDirectory(const QSettings& settings) {
    return settings.value("lastOpenedDirectory", QDir::currentPath()).toString();
}

void MainWindow::setLastOpenedDirectory(QSettings& settings, const QString filename) {
    QFileInfo fileInfo(filename);
    QString dir = fileInfo.absolutePath();
    settings.setValue("lastOpenedDirectory", dir);
}

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
    selectComboBoxOption(*ui->cbSoundMode, saveData->sound_mode);
    selectComboBoxOption(*ui->cbDifficulty, saveData->getFlag(SaveData::SAVE_FLAG_EASY | SaveData::SAVE_FLAG_NORMAL | SaveData::SAVE_FLAG_HARD));
    selectComboBoxOption(*ui->cbReinhardtEnding, saveData->getFlag(SaveData::SAVE_FLAG_REINDHART_GOOD_ENDING | SaveData::SAVE_FLAG_REINDHART_BAD_ENDING));
    selectComboBoxOption(*ui->cbReinhardtEnding, saveData->getFlag(SaveData::SAVE_FLAG_CARRIE_GOOD_ENDING | SaveData::SAVE_FLAG_CARRIE_BAD_ENDING));
    selectComboBoxOption(*ui->cbSubweapon, saveData->subweapon);

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
    ui->leMinutes->setText(QString::number(saveData->minute));
    ui->leSeconds->setText(QString::number(saveData->seconds));
    ui->leMilliseconds->setText(QString::number(saveData->milliseconds));
    ui->leFrameCount->setText(QString::number(saveData->gameplay_framecount));

    ui->leItemsSpecial1->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SPECIAL1)));
    ui->leItemsSpecial2->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_SPECIAL2)));
    ui->leItemsRoastChicken->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ROAST_CHICKEN)));
    ui->leItemsRoastBeef->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_ROAST_BEEF)));
    ui->leItemsHealingKit->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_HEALING_KIT)));
    ui->leItemsPurifying->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_PURIFYING)));
    ui->leItemsCureAmpoule->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_CURE_AMPOULE)));
    ui->leItemsPoutPourri->setText(QString::number(saveData->getItem(SaveData::ITEM_ID_POUT_POURRI)));
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

    ui->cboxHardMode->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_HARD_MODE_UNLOCKED));
    ui->cboxUseAlternateCostume->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_COSTUME_IS_BEING_USED));
    ui->cboxReinhardtCostume->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_HAVE_REINHARDT_ALT_COSTUME));
    ui->cboxCarrieCostume->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_HAVE_CARRIE_ALT_COSTUME));
    ui->cboxNitro->setChecked(saveData->getFlag(SaveData::SAVE_FLAG_CAN_EXPLODE_ON_JUMPING));
    ui->cboxVamp->setChecked(saveData->getFlag(SaveData::PLAYER_FLAG_VAMP));
    ui->cboxPoison->setChecked(saveData->getFlag(SaveData::PLAYER_FLAG_POISON));
    ui->cboxSto->setChecked(saveData->getFlag(SaveData::PLAYER_FLAG_STO));

    // Event flag grids. We edit each of the line edits associated to the event flags to assign the hex value gotten
    // from the save data. Then, the checkboxes will be ticked / unticked automatically
    for (unsigned int i = 0; i < NUM_EVENT_FLAGS; i++) {
        hexBitflagLineEdits[i]->setText(QString::number(saveData->getEventFlags(i)));
    }
}

void MainWindow::openFile(const QString& filename) {
    if (!filename.isEmpty()) {
        QFile file(filename);

        if (file.open(QIODevice::ReadOnly)) {
            SaveManager::getInstance()->parseAllSaveSlots(file, 0x30);
        }
        file.close();
    }
    //SaveManager::getInstance()->printAllSaves();
    // Populate with the first slot by default + main save
    populateMainWindow(&SaveManager::getInstance()->getSaves(0).main);
}

void MainWindow::fileOpenMenu() {
    // With these settings, we can obtain the last opened save directory
    QSettings settings("", "Castlevania 64 Save Editor");

    // Open file menu in the last opened directory by default
    QString filename = QFileDialog::getOpenFileName(this, "Open File", getLastOpenedDirectory(settings), "All accepted filetypes (*.pak, *.mpak, *.note);;Individual note (*.note);;Controller Pak data (*.pak, *.mpak);;All Files (*)");

    if (!filename.isEmpty()) {
        openFile(filename);
        setLastOpenedDirectory(settings, filename);
    }
}

void MainWindow::setupFileMenu() {
    // Setup the "Open > From File" button
    connect(ui->actionFrom_File, &QAction::triggered, this, &MainWindow::fileOpenMenu);

    // Setup the "Exit" button
    connect(ui->actionExit, &QAction::triggered, this, []() {
        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Exit", "Are you sure you want to quit?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            QApplication::exit();
        }
    });
}

void MainWindow::setupSlotMenu() {
    QMenu* menuSlot = menuBar()->addMenu("Slot");

    for (int i = 0; i < NUM_SAVES; ++i) {
        // Create a submenu for "Slot X"
        slotMenuOptions[i].slotOption = new QMenu(QString("Slot %1").arg(i + 1), this);
        menuSlot->addMenu(slotMenuOptions[i].slotOption);

        // Create "Main" action inside the Slot X menu
        slotMenuOptions[i].mainSaveOption = new QAction("Main", this);
        slotMenuOptions[i].mainSaveOption->setCheckable(true);
        slotMenuOptions[i].slotOption->addAction(slotMenuOptions[i].mainSaveOption);
        connect(slotMenuOptions[i].mainSaveOption, &QAction::triggered, this, [this, i]() {
            populateMainWindow(&SaveManager::getInstance()->getSaves(i).main);
            selectedSlot = i;
            isMain = true;
            updateSlotMenuCheckedState(i, true);
        });

        // Create "Beginning of Stage" action inside the Slot X menu
        slotMenuOptions[i].beginningOfStageSaveOption = new QAction("Beginning of Stage", this);
        slotMenuOptions[i].beginningOfStageSaveOption->setCheckable(true);
        slotMenuOptions[i].slotOption->addAction(slotMenuOptions[i].beginningOfStageSaveOption);
        connect(slotMenuOptions[i].beginningOfStageSaveOption, &QAction::triggered, this, [this, i]() {
            populateMainWindow(&SaveManager::getInstance()->getSaves(i).beginningOfStage);
            selectedSlot = i;
            isMain = false;
            updateSlotMenuCheckedState(i, false);
        });
    }
}

// When clicking on a slot option, check it, and *also uncheck* any other unselected options
// This is done for the "Main" and "Beginning of Stage" slot menu options
void MainWindow::updateSlotMenuCheckedState(int selectedSlotIndex, bool isMainSave) {
    for (unsigned int i = 0; i < NUM_SAVES; ++i) {
        slotMenuOptions[i].mainSaveOption->setChecked(i == selectedSlotIndex && isMainSave);
        slotMenuOptions[i].beginningOfStageSaveOption->setChecked(i == selectedSlotIndex && !isMainSave);
    }
}

void MainWindow::handleNumberOnlyInputUnsigned() {
    // Obtain the line edit object that called this function
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
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
        // to update the values for the Mandragora and Nitro accordingly
        // when editing either of those line edits
        checkMandragoraAndNitroLineEdits();

        lineEdit->setText(QString::number(value));
    }
}

// With this function, we can have more control during lineEdit initialization.
// In this case, we make it so that we can add a min and max value to each lineEdit without much copy-pasting
void MainWindow::setupLineEditNumberUnsigned(QLineEdit* lineEdit, const unsigned int minValue, const unsigned int maxValue) {
    // Using this regex, we can accept either only hex values (preceded "0x") or decimal values
    // of up to 8 digits long (to prevent them from overflowing the max int / uint)
    QRegularExpression acceptDecimalAndHexRegex(R"(^(\d{1,8}|0[xX][0-9A-Fa-f]{1,8})$)");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(acceptDecimalAndHexRegex, this);
    lineEdit->setValidator(validator);

    lineEdit->setProperty("minValue", minValue);
    lineEdit->setProperty("maxValue", maxValue);

    connect(lineEdit, &QLineEdit::editingFinished, this, &MainWindow::handleNumberOnlyInputUnsigned);
}

// Populate a Combo box given an array of name strings and their associated numeric value
void MainWindow::setupComboBox(QComboBox* comboBox, const Ui::ComboBoxData& array) {
    comboBox->clear();

    for (const auto& map: array) {
        for (const auto& entry: map) {
            comboBox->addItem(QString::fromStdString(entry.first), QVariant(entry.second));
        }
    }
}

void MainWindow::selectComboBoxOption(QComboBox& comboBox, const QVariant data) {
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

// The player can only have a Mandragora *OR* a Magical Nitro at the same time.
// This function checks if both Mandragora and Nitro have an amount larger than 0 at the same time.
// If so, set the amount for both items back to 0
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

QLineEdit* MainWindow::createGridFlag(QGridLayout* gridLayout, unsigned int flags) {
    QLineEdit* hexBitflagDisplay = new QLineEdit();
    hexBitflagDisplay->setAlignment(Qt::AlignRight);
    hexBitflagDisplay->setText(QString("0x%1").arg(flags, 8, 16, QChar('0')));

    // Ensure that we're putting "hexBitflagDisplay" in the right most part of the checkboxes, on the 1st row, 8th column
    // @note We also add an extra row and column for printing the column / row number
    gridLayout->addWidget(hexBitflagDisplay, 0, 9, 5, 1);
    // Add numbers on top and to the right showing the column / row number respectively
    for (unsigned int i = 0; i < 8; ++i) {
        QLabel* colLabel = new QLabel(QString::number(i));
        colLabel->setAlignment(Qt::AlignCenter);  // Center the row numbers
        gridLayout->addWidget(colLabel, 0, i + 1);
    }

    for (unsigned int i = 0; i < 4; ++i) {
        QLabel* rowLabel = new QLabel(QString::number(i));
        rowLabel->setAlignment(Qt::AlignCenter);  // Center the row numbers
        gridLayout->addWidget(rowLabel, i + 1, 0);
    }

    QVector<QCheckBox*> checkBoxes(32);

    // Create 32 checkboxes (one for each bit)
    for (unsigned int i = 0; i < 32; ++i) {
        QCheckBox* checkBox = new QCheckBox();
        // Set the checked state based on the flag value
        checkBox->setChecked(flags & (1 << i));
        // Add the checkbox to the grid layout. Then place it in a 4x8 grid
        // We add +1 so that we don't print the checkboxes in the same row / column as the numbers
        gridLayout->addWidget(checkBox, (i / 8) + 1, (i % 8) + 1);

        checkBoxes[i] = checkBox;
    }

    // Since now each checkbox is initialized, we can go ahead and connect them + add the function to handle them when toggled
    for (unsigned int j = 0; j < 32; j++) {
        // Make sure that each checkbox is updated *on-the-fly* as we edit the hex value from the line edit
        // To do so, we directly define a lambda function that does this for us. This is executed when pressing any of the checkboxes
        connect(checkBoxes[j], &QCheckBox::toggled, this, [checkBoxes, hexBitflagDisplay]() {
            unsigned int updatedFlags = 0;
            for (int j = 0; j < 32; ++j) {
                if (checkBoxes[j]->isChecked()) {
                    updatedFlags |= (1 << j);
                }
            }
            hexBitflagDisplay->setText(QString("0x%1").arg(updatedFlags, 8, 16, QChar('0')));
        });
    }

    // Lastly, we connect the "hexBitflagDisplay" and add a handling function that will update all
    // checkboxes depending on the hex bitflag value passed in the "hexBitflagDisplay" line edit
    connect(hexBitflagDisplay, &QLineEdit::textChanged, this, [checkBoxes, hexBitflagDisplay](const QString& text) {
        bool ok = false;
        unsigned int newFlags = text.toUInt(&ok, 16);

        // Ensure we limit the input value up to 0xFFFFFFFF
        if (ok && newFlags <= 0xFFFFFFFF) {
            for (int j = 0; j < 32; ++j) {
                checkBoxes[j]->setChecked(newFlags & (1 << j));
            }
        }
        else {
            hexBitflagDisplay->setText("0xffffffff");
        }
    });

    // We return "hexBitflagDisplay" so that we can access to it later for changing the event flag values by editing the lineEdit
    return hexBitflagDisplay;
}
