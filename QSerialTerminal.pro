#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T18:21:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSerialTerminal
TEMPLATE = app

DEFINES += __LANGAUGE_ENGLISH__

CONFIG += c++14

INCLUDEPATH += include/


SOURCES +=  src/main.cpp\
            src/mainwindow.cpp \
            src/qserialterminalicons.cpp \
            src/qserialterminalstrings.cpp \
            src/customaction.cpp \
            src/custommenu.cpp \
            src/qserialterminallineedit.cpp \
            src/generalutilities.cpp \
            src/serialport.cpp \
            src/tstream.cpp \
            src/tscriptexecutor.cpp \
            src/tscriptreader.cpp \
            src/fileutilities.cpp \
            src/systemcommand.cpp

HEADERS  += include/mainwindow.h\
            include/qserialterminalicons.h \
            include/qserialterminalstrings.h \
            include/customaction.h \
            include/eventtimer.h \
            include/tstream.h \
            include/tscriptexecutor.h \
            include/tscriptreader.h \
            include/serialport.h \
            include/generalutilities.h \
            include/fileutilities.h \
            include/qserialterminallineedit.h \
            include/custommenu.h \
            include/systemcommand.h

FORMS    += forms/mainwindow.ui

RESOURCES += resources/icons.qrc

android {
    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
