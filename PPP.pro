# ============================================================================
# PPP.pro
#
# Main project file. This is basically this project's Makefile, and defines the paths to all the project files.
# This project uses the Qt main, core, GUI for the main interface code. It also uses the network library
# and concurrent for database-related tasks. It uses the C++ 17 standard.
# ============================================================================

# Extra Qt needed libraries
QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Source code files
SOURCES += \
    src/windows/DatabaseSaveListActionButtonWindow.cpp \
    src/file/FileManager.cpp \
    src/file/FileLoader.cpp \
    src/database/DatabaseManager.cpp \
    src/database/Database.cpp \
    src/save/SaveManager.cpp \
    src/main.cpp \
    src/windows/MainWindow.cpp \
    src/windows/DatabaseMainWindow.cpp \
    src/windows/ControllerPakSelectionWindow.cpp

# Header files
HEADERS += \
    include/windows/Database/DatabaseSaveListActionButtonWindow.h \
    include/bit.h \
    include/file/FileManager.h \
    include/file/FileLoader.h \
    include/database/DatabaseManager.h \
    include/database/Database.h \
    include/save/Save.h \
    include/save/SaveManager.h \
    include/windows/ControllerPakSelection/ControllerPakSelectionWindow.h \
    include/windows/Database/DatabaseMainWindow.h \
    include/windows/main/MainWindow.h \
    include/windows/ComboBoxData.h

# Interface (UI) design files
FORMS += \
    ui/DatabaseSaveListActionButtonWindow.ui \
    ui/DatabaseMainWindow.ui \
    ui/MainWindow.ui \
    ui/ControllerPakSelectionWindow.ui


TRANSLATIONS += \


CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    Doxygen \
    README.md
