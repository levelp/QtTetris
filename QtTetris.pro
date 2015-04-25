# Файл проекта
TEMPLATE = app

CONFIG += c++11

# testlib - библиотека для модульного тестирования
QT       += testlib core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtTetris

SOURCES += main.cpp\
        mainwindow.cpp \
    gamesettings.cpp \
    about.cpp \
    game.cpp \
    settings.cpp \
    tests.cpp

HEADERS  += mainwindow.h \
    gamesettings.h \
    about.h \
    game.h \
    settings.h \
    tests.h

FORMS    += mainwindow.ui \
    gamesettings.ui \
    about.ui

DISTFILES += \
    README.md
