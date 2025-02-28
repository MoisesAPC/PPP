#include "include\windows\main\mainwindow.h"
#include "include/save/SaveManager.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

SaveManager* SaveManager::instance = nullptr;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    SaveManager::createInstance();

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

    SaveManager::destroyInstance();

    return result;
}
