QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/file/FileManager.cpp \
    src/file/FileLoader.cpp \
    src/database/DatabaseManager.cpp \
    src/database/Database.cpp \
    src/save/SaveManager.cpp \
    src/main.cpp \
    src/windows/mainwindow.cpp \
    src/windows/databaseaccesswindow.cpp \
    src/windows/controllerpakselectionwindow.cpp

HEADERS += \
    include/bit.h \
    include/file/FileManager.h \
    include/file/FileLoader.h \
    include/database/DatabaseManager.h \
    include/database/Database.h \
    include/save/Save.h \
    include/save/SaveManager.h \
    include/windows/controllerpakselection/controllerpakselectionwindow.h \
    include/windows/databaseaccess/databaseaccesswindow.h \
    include/windows/main/mainwindow.h \
    include/windows/main/slotMenu.h

FORMS += \
    ui/databaseaccesswindow.ui \
    ui/mainwindow.ui \
    ui/controllerpakselectionwindow.ui


TRANSLATIONS += \


CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    README.md
