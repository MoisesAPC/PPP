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

    setupLineEditNumber(ui->leLife, 0, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleNumberOnlyInput() {
    // Obtain the line edit object that called this function
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
    if (!lineEdit) {
        return;
    }

    QString text = lineEdit->text();
    bool ok;
    unsigned int value;

    if (text.startsWith("0x", Qt::CaseInsensitive)) {
        value = text.mid(2).toUInt(&ok, 16);
    }
    else {
        value = text.toUInt(&ok, 10);
    }

    if (ok) {
        int minValue = lineEdit->property("minValue").toUInt();
        int maxValue = lineEdit->property("maxValue").toUInt();

        value = qBound<unsigned int>(minValue, value, maxValue);

        lineEdit->setText(QString::number(value));
    }
}

// With this function, we can have more control during lineEdit initialization.
// In this case, we make it so that we can add a min and max value to each lineEdit without much copy-pasting
void MainWindow::setupLineEditNumber(QLineEdit* lineEdit, int minValue, int maxValue) {
    // Using this regex, we can accept either only hex values (preceded "0x") or decimal values
    QRegularExpression acceptDecimalAndHexRegex(QString("^(\\d{1,%1}|0[xX][0-9A-Fa-f]{1,%2})$")
                              .arg(QString::number(maxValue).length())
                              .arg(QString::number(maxValue, 16).length()));
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(acceptDecimalAndHexRegex, this);
    lineEdit->setValidator(validator);

    connect(lineEdit, &QLineEdit::editingFinished, this, &MainWindow::handleNumberOnlyInput);
}
