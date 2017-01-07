#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T18:21:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSerialTerminal
TEMPLATE = app

LD_LI

CONFIG += c++14 static

win32 {

    LIB_SOURCE_BASE = E:/GitHub/tjlutils
    INCLUDEPATH += include/ \
                   $${LIB_SOURCE_BASE}/serialport/include/ \
                   $${LIB_SOURCE_BASE}/generalutilities/include/ \
                   $${LIB_SOURCE_BASE}/systemcommand/include/ \
                   $${LIB_SOURCE_BASE}/fileutilities/include/ \
                   $${LIB_SOURCE_BASE}/eventtimer/include/ \
                   $${LIB_SOURCE_BASE}/tstream/include/ \
                   $${LIB_SOURCE_BASE}/prettyprinter/include/ \
                   $${LIB_SOURCE_BASE}/mathutilities/include/


    SOURCES +=  src/main.cpp\
                src/mainwindow.cpp \
                src/qserialterminalicons.cpp \
                src/qserialterminalstrings.cpp \
                src/customaction.cpp \
                $${LIB_SOURCE_BASE}/serialport/src/serialport.cpp \
                $${LIB_SOURCE_BASE}/generalutilities/src/generalutilities.cpp \
                $${LIB_SOURCE_BASE}/systemcommand/src/systemcommand.cpp \
                $${LIB_SOURCE_BASE}/fileutilities/src/fileutilities.cpp \
                $${LIB_SOURCE_BASE}/eventtimer/src/eventtimer.cpp \
                $${LIB_SOURCE_BASE}/prettyprinter/src/prettyprinter.cpp \
                $${LIB_SOURCE_BASE}/mathutilities/src/mathutilities.cpp \
                $${LIB_SOURCE_BASE}/tstream/src/tscriptexecutor.cpp \
                $${LIB_SOURCE_BASE}/tstream/src/tscriptreader.cpp

    HEADERS  += include/mainwindow.h\
                include/qserialterminalicons.h \
                include/qserialterminalstrings.h \
                include/customaction.h \
                $${LIB_SOURCE_BASE}/serialport/include/serialport.h \
                $${LIB_SOURCE_BASE}/generalutilities/include/generalutilities.h \
                $${LIB_SOURCE_BASE}/systemcommand/include/systemcommand.h \
                $${LIB_SOURCE_BASE}/fileutilities/include/fileutilities.h \
                $${LIB_SOURCE_BASE}/fileutilities/include/win32-dirent.h \
                $${LIB_SOURCE_BASE}/eventtimer/include/eventtimer.h \
                $${LIB_SOURCE_BASE}/tstream/include/tstream.h \
                $${LIB_SOURCE_BASE}/tstream/include/tscriptreader.h \
                $${LIB_SOURCE_BASE}/tstream/include/tscriptexecutor.h \
                $${LIB_SOURCE_BASE}/prettyprinter/include/prettyprinter.h \
                $${LIB_SOURCE_BASE}/mathutilities/include/mathutilities.h

    FORMS    += forms/mainwindow.ui

    RESOURCES += resources/icons.qrc
    RC_FILE += resources/icons/qserialterminal.rc
}

unix:!android {

    INCLUDEPATH += include/


    SOURCES +=  src/main.cpp\
                src/mainwindow.cpp \
                src/qserialterminalicons.cpp \
                src/qserialterminalstrings.cpp \
                src/customaction.cpp


    HEADERS  += include/mainwindow.h\
                include/qserialterminalicons.h \
                include/qserialterminalstrings.h \
                include/customaction.h

    FORMS    += forms/mainwindow.ui

    RESOURCES += resources/icons.qrc

    CONFIG(release, debug|release): LIBS += -L$$PWD/C:/Cygwin64/opt/LibraryBuilds/tjlutils/Debug/debug/release/ -ltjlutils
    LIBS += -L$$PWD/C:/Cygwin64/opt/LibraryBuilds/tjlutils/Debug/ -ltjlutils

    INCLUDEPATH += $$PWD/C:/Cygwin64/opt/LibraryBuilds/tjlutils/Debug/debug
    DEPENDPATH += $$PWD/C:/Cygwin64/opt/LibraryBuilds/tjlutils/Debug/debug
}

android {
    LIB_SOURCE_BASE = E:/opt/SharedStorage/GitHub/tjlutils

    INCLUDEPATH += include/ \
                   $${LIB_SOURCE_BASE}/serialport/include/ \
                   $${LIB_SOURCE_BASE}/generalutilities/include/ \
                   $${LIB_SOURCE_BASE}/systemcommand/include/ \
                   $${LIB_SOURCE_BASE}/fileutilities/include/ \
                   $${LIB_SOURCE_BASE}/eventtimer/include/ \
                   $${LIB_SOURCE_BASE}/tstream/include/ \
                   $${LIB_SOURCE_BASE}/systemcommand/include/ \
                   $${LIB_SOURCE_BASE}/mathutilities/include/


    SOURCES +=  src/main.cpp\
                src/mainwindow.cpp \
                src/qserialterminalicons.cpp \
                src/qserialterminalstrings.cpp \
                src/customaction.cpp \
                $${LIB_SOURCE_BASE}/serialport/src/serialport.cpp \
                $${LIB_SOURCE_BASE}/generalutilities/src/generalutilities.cpp \
                $${LIB_SOURCE_BASE}/fileutilities/src/fileutilities.cpp \
                $${LIB_SOURCE_BASE}/eventtimer/src/eventtimer.cpp \
                $${LIB_SOURCE_BASE}/tstream/src/tscriptreader.cpp \
                $${LIB_SOURCE_BASE}/tstream/src/tscriptexecutor.cpp \
                $${LIB_SOURCE_BASE}/systemcommand/src/systemcommand.cpp \
                $${LIB_SOURCE_BASE}/mathutilities/src/mathutilities.cpp

    HEADERS  += include/mainwindow.h\
                include/qserialterminalicons.h \
                include/qserialterminalstrings.h \
                include/customaction.h \
                $${LIB_SOURCE_BASE}/serialport/include/serialport.h \
                $${LIB_SOURCE_BASE}/generalutilities/include/generalutilities.h \
                $${LIB_SOURCE_BASE}/systemcommand/include/systemcommand.h \
                $${LIB_SOURCE_BASE}/fileutilities/include/fileutilities.h \
                $${LIB_SOURCE_BASE}/fileutilities/include/win32-dirent.h \
                $${LIB_SOURCE_BASE}/eventtimer/include/eventtimer.h \
                $${LIB_SOURCE_BASE}/tstream/include/tstream.h \
                $${LIB_SOURCE_BASE}/tstream/include/tscriptreader.h \
                $${LIB_SOURCE_BASE}/tstream/include/tscriptexecutor.h \
                $${LIB_SOURCE_BASE}/systemcommand/include/systemcommand.h \
                $${LIB_SOURCE_BASE}/mathutilities/include/mathutilities.h

    FORMS    += forms/mainwindow.ui

    RESOURCES += resources/icons.qrc

}

HEADERS += \
    include/custommenu.h \
    include/qserialterminallineedit.h

SOURCES += \
    src/custommenu.cpp \
    src/qserialterminallineedit.cpp

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
