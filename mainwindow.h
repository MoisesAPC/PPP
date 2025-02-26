#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <map>

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
    void handleNumberOnlyInputUnsigned();
    void setupLineEditNumberUnsigned(QLineEdit* lineEdit, unsigned int minValue, unsigned int maxValue);
    void setupComboBox(QComboBox* comboBox, const std::vector<std::map<std::string, int> >& array);

private:
    Ui::MainWindow* ui;

    std::vector<std::map<std::string, int>> comboBoxDataMap = {
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

    std::vector<std::map<std::string, int>> comboBoxDataCharacter = {
        {{"Reinhardt", 0}},
        {{"Carrie", 1}}
    };

    std::vector<std::map<std::string, int>> comboBoxDataButtonConfig = {
        {{"Type A", 0}},
        {{"Type B", 1}},
        {{"Type C", 2}}
    };

    std::vector<std::map<std::string, int>> comboBoxDataSoundMode = {
        {{"Stereo", 0}},
        {{"Monoaural", 1}}
    };

    std::vector<std::map<std::string, int>> comboBoxDataSubweapon = {
        {{"None", 0}},
        {{"Knife", 1}},
        {{"Holy Water", 2}},
        {{"Cross", 3}},
        {{"Axe", 4}}
    };
};
#endif // MAINWINDOW_H
