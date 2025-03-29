#include "include\windows\databaseaccess\databaseaccesswindow.h"
#include "include\windows\databaseaccess\databasesavelistactionwindow.h"
#include "ui_databaseaccesswindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QtGlobal>

DatabaseAccessWindow::DatabaseAccessWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseAccessWindow)
{
    ui->setupUi(this);

    switchPage(ui->stackedWidgetPages, ui->pageAccess);

    setupConnectMenu();
    setupSaveListMenu();
}

void DatabaseAccessWindow::setupConnectMenu() {
    setupComboBox(ui->cbDatabase, comboBoxDataDatabaseTypes,
        [](int value) { DatabaseManager::getInstance()->setDatabaseType(value); }
    );

    setupLineEditHostname(ui->leHostname);
    ui->sbPort->setRange(0, 65535);

    connect(ui->leUsername, &QLineEdit::editingFinished, this, [this]() {
        DatabaseManager::getInstance()->setUsername(ui->leUsername->text());
    });

    connect(ui->lePassword, &QLineEdit::editingFinished, this, [this]() {
        DatabaseManager::getInstance()->setPassword(ui->lePassword->text());
    });

    // Setup default values for these fields
    // by using the default CouchDB values
    DatabaseManager::getInstance()->setDatabaseType(DatabaseManager::DATABASE_COUCHDB);
    ui->leHostname->setText("localhost");
    ui->sbPort->setValue(5984);

    connect(ui->buttonConnect, &QPushButton::clicked, this, [this]() {
        onConnectButtonPress();
    });
}

void DatabaseAccessWindow::setupSaveListMenu() {
    connect(ui->buttonUpload, &QPushButton::clicked, this, &DatabaseAccessWindow::onUploadSaveButtonPress);
    connect(ui->sbPageList, &QSpinBox::valueChanged, this, &DatabaseAccessWindow::onPageSwitch);
}

void DatabaseAccessWindow::createSaveListButtons() {
    clearSaveList();

    if (saveEntries.empty()) {
        return;
    }
    // Depending on the value gotten from the spinbox, we decide what parts of the "saveEntries" array we have to render
    // This is done to properly implement page switching using the spinbox
    const int startIndex = (ui->sbPageList->value() - 1) * entriesPerPage;
    const int endIndex = qMin(startIndex + entriesPerPage, static_cast<int>(saveEntries.size()));

    for (int i = startIndex; i < endIndex; i++) {
        QPushButton* actionButton = new QPushButton();

        setSaveListButtonProperties(actionButton, saveEntries[i].documentId, i + 1, saveEntries[i].region, saveEntries[i].rev);
        ui->buttonListLayout->addWidget(actionButton);

        connect(actionButton, &QPushButton::clicked, this, [this, actionButton]() {
            QString documentId = actionButton->property("documentId").toString();
            QString rev = actionButton->property("rev").toString();
            onActionButtonClicked(documentId, rev);
        });
    }
}

// Removes all buttons from the save list
void DatabaseAccessWindow::clearSaveList() {
    while (QLayoutItem* item = ui->buttonListLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            delete widget;
        }
        delete item;
    }
}

void DatabaseAccessWindow::setSaveListButtonProperties(QPushButton* button, const QString& documentId, const int listIndex, const int region, const QString& rev) {
    button->setProperty("documentId", documentId);
    button->setProperty("rev", rev);
    button->setProperty("listIndex", listIndex);

    QString regionString = "";
    switch (region) {
        default:
        case SaveData::USA:
            regionString = "USA";
            break;

        case SaveData::JPN:
            regionString = "JPN";
            break;

        case SaveData::PAL:
            regionString = "PAL";
            break;
    }

    button->setProperty("region", regionString);

    button->setText(QString("Save (%1):\n%2\n%3")
                        .arg(listIndex)
                        .arg(documentId)
                        .arg(regionString));
}

DatabaseAccessWindow::~DatabaseAccessWindow() {
    DatabaseManager::getInstance()->getDatabase()->disconnectFromDatabase();

    delete ui;
}

// Populate a Combo box given an array of name strings and their associated numeric value
void DatabaseAccessWindow::setupComboBox(QComboBox* comboBox, const Ui::ComboBoxData& array, std::function<void(int)> setter) {
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

// This function ensures that the line edit can only accept certain hostnames based on the rules below
void DatabaseAccessWindow::setupLineEditHostname(QLineEdit* lineEdit) {
    // Regular expression to validate hostnames:
    // - Allowed characters: letters (a-z, A-Z), numbers (0-9), hyphens (-), and periods (.).
    // - Cannot start or end with a hyphen or period.
    // - Each label (separated by dots) must be 1-63 characters long.
    // - Total length of hostname must not exceed 253 characters.
    QRegularExpression acceptHostnameRegex(R"(^[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?)*$)");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(acceptHostnameRegex, this);
    lineEdit->setValidator(validator);

    connect(lineEdit, &QLineEdit::editingFinished, this, [lineEdit]() {
        QString text = lineEdit->text();
        int pos = 0;

        if (lineEdit->validator()->validate(text, pos) == QValidator::Acceptable) {
            lineEdit->setText(text);
        }
        else {
            lineEdit->clear();
        }
    });
}

void DatabaseAccessWindow::switchPage(QStackedWidget* stackedWidget, const QWidget* page) {
    stackedWidget->setCurrentIndex(stackedWidget->indexOf(page));
}

void DatabaseAccessWindow::onConnectButtonPress() {
    DatabaseManager::getInstance()->assignDatabase();
    Database* database = DatabaseManager::getInstance()->getDatabase();

    // First, grab the parameters from the UI
    database->setHostname(ui->leHostname->text());
    database->setPort(ui->sbPort->value());
    database->setDatabaseName(ui->leDatabaseName->text());

    if (database->getHostname() == "" || database->getPort() > 65535 || database->getDatabaseName() == "") {
        QMessageBox::critical(this, "", "Please, make sure that what was written in the form is valid.");
        return;
    }

    // Finally, try to connect to the database
    // What's returned from the work function gets stored in the "result" variable
    bool result = DatabaseManager::getInstance()->connectToDatabase();

    if (result == true) {
        // Switch to the save list page
        QMessageBox::information(this, "", "Successfully connected to the database!");

        createSaveList();
    }
    else {
        // Disconnect from the database on error
        QMessageBox::critical(this, "", "Error while connecting to the database.");
        DatabaseManager::getInstance()->getDatabase()->disconnectFromDatabase();
    }
}

// Retrieve save list entries from the database and construct the button list with the retrieved data
void DatabaseAccessWindow::createSaveList() {
    saveEntries.clear();
    saveEntries = DatabaseManager::getInstance()->getAllEntries();

    int maxPage = (saveEntries.size() + entriesPerPage - 1) / entriesPerPage;
    ui->sbPageList->setMaximum(maxPage);

    //Start on page 1
    ui->sbPageList->setValue(1);

    switchPage(ui->stackedWidgetPages, ui->pageSaveList);

    createSaveListButtons();
}

void DatabaseAccessWindow::onUploadSaveButtonPress() {
    std::vector<SaveSlot> entries;

    // Only allow enabled saves to be uploaded to the database
    if (SaveManager::getInstance()->areAllSavesDisabled()) {
        QMessageBox::critical(this, "", "The current save is empty, so it can't be added to the database");
        return;
    }

    bool ok;
    QString documentId = QInputDialog::getText(this, "Introduce name", "Enter an ID name for the save: ", QLineEdit::Normal, "", &ok);
    QString rev = DatabaseManager::getInstance()->getDocumentRevision(documentId);

    if (ok && !documentId.isEmpty()) {
        for (int i = 0; i < NUM_SAVES; i++) {
            entries.push_back(SaveManager::getInstance()->getSaveSlot(i));
        }

        DatabaseManager::getInstance()->createEntry(documentId, entries, rev);
        createSaveList();
    }
}

void DatabaseAccessWindow::onPageSwitch() {
    createSaveListButtons();
}

void DatabaseAccessWindow::onActionButtonClicked(const QString& docId, const QString& rev) {
    // @note We have to pass "DatabaseAccessWindow" as a parent of "DatabaseSaveListActionWindow",
    // in order for the "DatabaseSaveListActionWindow" signals to work
    DatabaseSaveListActionWindow* actionWindow = new DatabaseSaveListActionWindow(docId, rev, this);

    // If "reloadSaveList" is true, it means an item was deleted from the database, so we have to reload the save list
    connect(actionWindow, &DatabaseSaveListActionWindow::deleteConfirmed, this, [this](bool reloadSaveList) {
        if (reloadSaveList) {
            createSaveList();
        }
    });

    // If "backToMainWindow" is true, it means an item was deleted from the database, so we have to reload the save list
    connect(actionWindow, &DatabaseSaveListActionWindow::editConfirmed, this, [this](bool backToMainWindow) {
        if (backToMainWindow) {
            this->close();
        }
    });

    actionWindow->exec();
}
