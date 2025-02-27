#include "include\windows\main\mainwindow.h"
#include <QIntValidator>    // With "QIntValidator", we can validate the contents of an integer (see "handleNumberOnlyInput()")
#include <QtGlobal>         // qBound()
#include <climits>          // SHRT_MAX, UINT_MAX, etc

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize pages
    setupPageMain();
    setupPageItems();
    setupPageEventFlags();

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
    setupLineEditNumberUnsigned(ui->lePowerup, 0, 2);

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
    createGridFlag(ui->gridFlagSet0, 0);

    ui->labelSet1->setText("Set 1 (Villa Foyer, Villa Hallway)");
    createGridFlag(ui->gridFlagSet1, 0);

    ui->labelSet2->setText("Set 2 (Underground Waterway, Castle Center - Top elevator room)");
    createGridFlag(ui->gridFlagSet2, 0);

    /* Page 2 */
    ui->labelSet3->setText("Set 3 (Castle Center - Friendly lizard man, Castle Center - Nitro room)");
    createGridFlag(ui->gridFlagSet3, 0);

    ui->labelSet4->setText("Set 4 (Tower of Execution, Tower of Sorcery, Duel Tower)");
    createGridFlag(ui->gridFlagSet4, 0);

    ui->labelSet5->setText("Set 5 (Castle Keep - Stairs, Castle Keep, Clock Tower)");
    createGridFlag(ui->gridFlagSet5, 0);

    /* Page 3 */
    ui->labelSet6->setText("Set 6 (Dracula Desert, Rose / Actriese room, Room of Clocks)");
    createGridFlag(ui->gridFlagSet6, 0);

    ui->labelSet7->setText("Set 7 (Tower of Science - Turrets)");
    createGridFlag(ui->gridFlagSet7, 0);

    ui->labelSet8->setText("Set 8 (Castle Center - Bottom Elevator, Castle Center - Gears room)");
    createGridFlag(ui->gridFlagSet8, 0);

    /* Page 4 */
    ui->labelSet9->setText("Set 9 (Villa - Front Yard)");
    createGridFlag(ui->gridFlagSet9, 0);

    ui->labelSet10->setText("Set 10 (Castle Wall - Main)");
    createGridFlag(ui->gridFlagSet10, 0);

    ui->labelSet11->setText("Set 11 (Maze Garden, Castle Center - Library)");
    createGridFlag(ui->gridFlagSet11, 0);

    /* Page 5 */
    ui->labelSet12->setText("Set 12 (Tunnel)");
    createGridFlag(ui->gridFlagSet12, 0);

    ui->labelSet13->setText("Set 13 (Castle Center - Main)");
    createGridFlag(ui->gridFlagSet13, 0);

    ui->labelSet14->setText("Set 14 (Castle Wall - Towers)");
    createGridFlag(ui->gridFlagSet14, 0);

    /* Page 6 */
    ui->labelSet15->setText("Set 15 (Tower of Science)");
    createGridFlag(ui->gridFlagSet15, 0);


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

void MainWindow::createGridFlag(QGridLayout* gridLayout, unsigned int flags) {
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
}
