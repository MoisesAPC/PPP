/**
 * @file main.cpp
 * @brief Main source code file
 *
 * This file contains the main entrypoint function of the program.
 *
 * @author Moisés Antonio Pestano Castro
 */

#include "include/windows/main/MainWindow.h"
#include "include/save/SaveManager.h"
#include "include/file/FileManager.h"
#include "include/database/DatabaseManager.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

/**
 * This program uses the Singleton pattern to handle different kind of tasks
 * from any part of the program.
 *
 * Since these are static, these will live for the entire lifetime of the application.
 */
SaveManager* SaveManager::instance = nullptr;
FileManager* FileManager::instance = nullptr;
DatabaseManager* DatabaseManager::instance = nullptr;

void createSingletons() {
    SaveManager::createInstance();
    FileManager::createInstance();
    DatabaseManager::createInstance();
}

void destroySingletons() {
    SaveManager::destroyInstance();
    FileManager::destroyInstance();
    DatabaseManager::destroyInstance();
}

int main(int argc, char *argv[]) {
    // Create the application and show the main window
    QApplication a(argc, argv);

    createSingletons();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PPP_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();

    int result = a.exec();

    destroySingletons();

    return result;
}
