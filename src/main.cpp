#include "include/windows/main/MainWindow.h"
#include "include/save/SaveManager.h"
#include "include/file/FileManager.h"
#include "include/database/DatabaseManager.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

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
