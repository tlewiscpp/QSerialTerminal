#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T17:48:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSerialTerminal
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/


SOURCES += \
    src/Main.cpp \
src/ApplicationSettings.cpp \
src/ApplicationSettingsLoader.cpp \
src/MainWindow.cpp \
src/ApplicationIcons.cpp \
src/ApplicationStrings.cpp \
src/CustomAction.cpp \
src/QSerialTerminalLineEdit.cpp \
src/ApplicationUtilities.cpp \
src/AboutApplicationWidget.cpp

HEADERS += \
    src/GlobalDefinitions.h \
    src/ApplicationSettings.h \
    src/ApplicationSettingsLoader.h \
    src/MainWindow.h \
    src/ApplicationIcons.h \
    src/ApplicationStrings.h \
    src/CustomAction.h \
    src/QSerialTerminalLineEdit.h \
    src/ApplicationUtilities.h \
    src/SerialPort.h \
    src/IByteStream.h \
    src/AboutApplicationWidget.h \
    src/Version.h

FORMS += \
        forms/MainWindow.ui \
        forms/AboutApplicationWidget.ui

RESOURCES += \
    resources/icons.qrc \
    resources/translations.qrc


