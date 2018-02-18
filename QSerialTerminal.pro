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

win32:CONFIG += win
win64:CONFIG += win

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCE_ROOT = src
FORMS_ROOT = forms
RESOURCES_ROOT = resources
THIRD_PARTY_ROOT= third-party
CPP_SERIAL_PORT_ROOT = $${THIRD_PARTY_ROOT}/CppSerialPort/CppSerialPort/CppSerialPort/

INCLUDEPATH += $${SOURCE_ROOT}

SOURCES += \
    $${SOURCE_ROOT}/Main.cpp \
    $${SOURCE_ROOT}/ApplicationSettings.cpp \
    $${SOURCE_ROOT}/ApplicationSettingsLoader.cpp \
    $${SOURCE_ROOT}/MainWindow.cpp \
    $${SOURCE_ROOT}/ApplicationIcons.cpp \
    $${SOURCE_ROOT}/QSerialTerminalLineEdit.cpp \
    $${SOURCE_ROOT}/ApplicationUtilities.cpp \
    $${SOURCE_ROOT}/SingleInstanceGuard.cpp \
    $${SOURCE_ROOT}/AboutApplicationWidget.cpp \
    $${CPP_SERIAL_PORT_ROOT}/SerialPort.cpp \
    $${CPP_SERIAL_PORT_ROOT}/IByteStream.cpp

HEADERS += \
    $${SOURCE_ROOT}/GlobalDefinitions.h \
    $${SOURCE_ROOT}/ApplicationSettings.h\ \
    $${SOURCE_ROOT}/ApplicationSettingsLoader.h \
    $${SOURCE_ROOT}/MainWindow.h \
    $${SOURCE_ROOT}/ApplicationIcons.h \
    $${SOURCE_ROOT}/QSerialTerminalLineEdit.h \
    $${SOURCE_ROOT}/ApplicationUtilities.h \
    $${SOURCE_ROOT}/AboutApplicationWidget.h \
    $${SOURCE_ROOT}/SingleInstanceGuard.h \
    $${SOURCE_ROOT}/QActionSetDefs.h \
    $${SOURCE_ROOT}/ApplicationStrings.h \
    $${SOURCE_ROOT}/Version.h \
    $${CPP_SERIAL_PORT_ROOT}/SerialPort.cpp \
    $${CPP_SERIAL_PORT_ROOT}/IByteStream.cpp

FORMS += \
    $${FORMS_ROOT}/MainWindow.ui \
    $${FORMS_ROOT}/AboutApplicationWidget.ui

RESOURCES += \
    $${RESOURCES_ROOT}/icons.qrc

win {
    GETOPT_ROOT = $${THIRD_PARTY_ROOT}/win32/getopt/
    SOURCES += $${GETOPT_ROOT}/getopt.c
    HEADERS += $${GETOPT_ROOT}/getopt.h
}
