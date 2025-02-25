#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void handleNumberOnlyInput();
    void setupLineEditNumber(QLineEdit* lineEdit, int minValue, int maxValue);

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
