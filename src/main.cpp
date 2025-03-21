#include "include\windows\main\mainwindow.h"
#include "include/save/SaveManager.h"
#include "include/file/FileManager.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

SaveManager* SaveManager::instance = nullptr;
FileManager* FileManager::instance = nullptr;

void createSingletons() {
    SaveManager::createInstance();
    FileManager::createInstance();
}

void destroySingletons() {
    SaveManager::destroyInstance();
    FileManager::destroyInstance();
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
