#include "include\windows\databaseaccess\databaseaccesswindow.h"
#include "ui_databaseaccesswindow.h"
#include <QProgressDialog>
#include <QMessageBox>
#include <QtConcurrent>
#include <QFuture>

DatabaseAccessWindow::DatabaseAccessWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseAccessWindow)
{
    ui->setupUi(this);

    setupComboBox(ui->cbDatabase, comboBoxDataDatabaseTypes,
        [](int value) { DatabaseManager::getInstance()->setDatabaseType(value); }
    );

    setupLineEditHostname(ui->leHostname);
    ui->sbPort->setRange(0, 65535);

    // Setup default values for both of these fields
    // by using the default CouchDB values
    ui->leHostname->setText("localhost");
    ui->sbPort->setValue(5984);

    connect(ui->buttonConnect, &QPushButton::clicked, this, [this]() {
        onConnectButtonPress();
    });
}

DatabaseAccessWindow::~DatabaseAccessWindow() {
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

// Shows a loading bar while it does the "workFunction" (passed as a function pointer)
void DatabaseAccessWindow::showLoadingBar(const QString& messageText, const QString& titleText, const QString& successMessage, const QString& errorMessage, std::function<bool()> workFunction) {
    bool result = 0;

    QProgressDialog loadingBar(messageText, "", 0, 100, this);
    loadingBar.setWindowModality(Qt::WindowModal);
    loadingBar.setWindowTitle("Please Wait");
    loadingBar.setMinimumDuration(0);
    loadingBar.setValue(0);
    loadingBar.show();

    // We run the work function asynchronously,
    // then wait until the function is finished executing
    QFuture<bool> operation = QtConcurrent::run(workFunction);

    while (!operation.isFinished()) {}

    loadingBar.close();

    // What's returned from the work function gets stored in the "result" variable
    result = operation.result();
    if (result == true) {
        // Switch to the save list page
        QMessageBox::information(this, "", successMessage);
        switchPage(ui->stackedWidgetPages, ui->pageSaveList);
    }
    else {
        // Disconnect from the database on error
        QMessageBox::critical(this, "", errorMessage);
        DatabaseManager::getInstance()->getDatabase()->disconnectFromDatabase();
    }
}

void DatabaseAccessWindow::switchPage(QStackedWidget* stackedWidget, const QWidget* page) {
    stackedWidget->setCurrentIndex(stackedWidget->indexOf(page));
}

void DatabaseAccessWindow::onConnectButtonPress() {
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
    showLoadingBar("Please wait...", "Connecting to database", "Successfully connected to the database!", "An error has occured while connecting.", []() -> bool {
        return DatabaseManager::getInstance()->connectToDatabase();
    });
}
