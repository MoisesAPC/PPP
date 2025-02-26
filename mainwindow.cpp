#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>    // With "QIntValidator", we can validate the contents of an integer (see "handleNumberOnlyInput()")
#include <QtGlobal>         // qBound()
#include <climits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize line edits
    setupLineEditNumberUnsigned(ui->leLife, 0, 100);
    setupLineEditNumberUnsigned(ui->leGold, 0, 99999);

    setupLineEditNumberUnsigned(ui->leSpawn, 0, SHRT_MAX);
    setupLineEditNumberUnsigned(ui->leWhiteJewel, 0, UCHAR_MAX);

    setupLineEditNumberUnsigned(ui->leTimesSaved, 0, UINT_MAX);
    setupLineEditNumberUnsigned(ui->leDeathCount, 0, UINT_MAX);
    setupLineEditNumberUnsigned(ui->leGoldRenon, 0, UINT_MAX);

    setupLineEditNumberUnsigned(ui->leWeek, 0, SHRT_MAX);
    setupLineEditNumberUnsigned(ui->leDay, 0, 7 - 1);
    setupLineEditNumberUnsigned(ui->leHour, 0, 24 - 1);
    setupLineEditNumberUnsigned(ui->leMinutes, 0, 60 - 1);
    setupLineEditNumberUnsigned(ui->leSeconds, 0, 60 - 1);
    setupLineEditNumberUnsigned(ui->leMilliseconds, 0, 600 - 1);
    setupLineEditNumberUnsigned(ui->leFrameCount, 0, UINT_MAX);
}

MainWindow::~MainWindow()
{
    delete ui;
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
