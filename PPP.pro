QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/SaveManager.cpp \
    src/main.cpp \
    src/windows/mainwindow.cpp

HEADERS += \
    include/bit.h \
    include/save/Save.h \
    include/save/SaveManager.h \
    include/windows/main/mainwindow.h

FORMS += \
    ui/mainwindow.ui

TRANSLATIONS += \
    ui/languages/PPP_en_US.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    README.md
