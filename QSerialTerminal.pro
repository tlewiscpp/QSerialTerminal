#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T18:21:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -ltjlutils

TARGET = QSerialTerminal
TEMPLATE = app

CONFIG += c++14

INCLUDEPATH += include/

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
            src/qserialterminalicons.cpp \
            src/qserialterminalstrings.cpp \
            src/qserialterminalutilities.cpp \
            src/serialterminallineedit.cpp \
            src/scriptreader.cpp \
            src/scriptexecutor.cpp \
            src/serialcommand.cpp \
    src/customaction.cpp

HEADERS  += include/mainwindow.h\
    include/qserialterminalicons.h \
    include/qserialterminalstrings.h \
    include/qserialterminalutilities.h \
    include/serialterminallineedit.h \
    include/scriptreader.h \
    include/scriptexecutor.h \
    include/serialcommand.h \
    include/customaction.h

FORMS    += forms/mainwindow.ui

RESOURCES += resources/icons.qrc

win32:RC_FILE += resources/icons/qserialterminal.rc
