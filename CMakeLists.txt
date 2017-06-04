#CMakeLists.txt -> QSerialTerminal

cmake_minimum_required (VERSION 3.2) 
project(QSerialTerminal CXX)

set(CMAKE_BUILD_TYPE Debug)
set(SOURCE_ROOT ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set (X86 TRUE)
else()
    set (X86 FALSE)
endif()

if (CYGWIN OR WIN32 OR (NOT X86))
    set(CMAKE_AUTORCC ON)
    set(GCC_ADDITIONAL_COMPILE_FLAGS "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
else()
    set(GCC_ADDITIONAL_COMPILE_FLAGS "-fsanitize=undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
endif()

set (QSERIALTERMINAL_SOURCES "${SOURCE_ROOT}/src/main.cpp"
                            "${SOURCE_ROOT}/src/mainwindow.cpp"
                            "${SOURCE_ROOT}/src/qserialterminalicons.cpp"
                            "${SOURCE_ROOT}/src/qserialterminalstrings.cpp"
                            "${SOURCE_ROOT}/src/customaction.cpp"
                            "${SOURCE_ROOT}/src/custommenu.cpp"
                            "${SOURCE_ROOT}/src/qserialterminallineedit.cpp"
                            "${SOURCE_ROOT}/src/generalutilities.cpp"
                            "${SOURCE_ROOT}/src/serialport.cpp"
                            "${SOURCE_ROOT}/src/tstream.cpp"
                            "${SOURCE_ROOT}/src/tscriptexecutor.cpp"
                            "${SOURCE_ROOT}/src/tscriptreader.cpp"
                            "${SOURCE_ROOT}/src/fileutilities.cpp"
                            "${SOURCE_ROOT}/src/systemcommand.cpp")

set (QSERIALTERMINAL_HEADERS "${SOURCE_ROOT}/src/mainwindow.h"
                             "${SOURCE_ROOT}/src/qserialterminalicons.h"
                             "${SOURCE_ROOT}/src/qserialterminalstrings.h"
                             "${SOURCE_ROOT}/src/customaction.h"
                             "${SOURCE_ROOT}/src/eventtimer.h"
                             "${SOURCE_ROOT}/src/tstream.h"
                             "${SOURCE_ROOT}/src/tscriptexecutor.h"
                             "${SOURCE_ROOT}/src/tscriptreader.h"
                             "${SOURCE_ROOT}/src/serialport.h"
                             "${SOURCE_ROOT}/src/generalutilities.h"
                             "${SOURCE_ROOT}/src/fileutilities.h"
                             "${SOURCE_ROOT}/src/qserialterminallineedit.h"
                             "${SOURCE_ROOT}/src/custommenu.h"
                             "${SOURCE_ROOT}/src/systemcommand.h")
if (WIN32)
    set (QSERIALTERMINAL_HEADERS "${QSERIALTERMINAL_HEADERS} ${SOURCE_ROOT}/src/version.h")
endif()

set (QSERIALTERMINAL_FORMS "${SOURCE_ROOT}/forms/mainwindow.ui")
set (QSERIALTERMINAL_RESOURCES "${SOURCE_ROOT}/resources/icons.qrc")

qt5_wrap_cpp(QSERIALTERMINAL_HEADERS_MOC "${QSERIALTERMINAL_HEADERS}")
qt5_wrap_ui (QSERIALTERMINAL_FORMS_MOC  "${QSERIALTERMINAL_FORMS}")
qt5_add_resources(QMINESWEEPER_RESOURCES_RCC "${QSERIALTERMINAL_RESOURCES}")

add_executable(QSerialTerminal "${QSERIALTERMINAL_SOURCES}"
                               "${QSERIALTERMINAL_HEADERS}"
                               "${QSERIALTERMINAL_FORMS}"
                               "${QSERIALTERMINAL_RESOURCES}")

target_link_libraries (QSerialTerminal Qt5::Widgets Qt5::Gui Qt5::Core pthread)
