cmake_minimum_required (VERSION 3.1)
set (PROJECT_NAME QSerialTerminal)
project(${PROJECT_NAME} CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(src/)

set (QSERIALTERMINAL_SOURCES 
    src/Main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/ApplicationIcons.cpp src/ApplicationIcons.h
    src/ApplicationStrings.cpp src/ApplicationStrings.h
    src/CustomAction.cpp src/CustomAction.h
    src/CustomMenu.cpp src/CustoMenu.h
    src/QSerialTerminalLineEdit.cpp src/QSerialTerminalLineEdit.h
    src/ApplicationUtilities.cpp src/ApplicationUtilities.h
    src/SerialPort.cpp src/SerialPort.h
    src/IByteStream.cpp src/IByteStream.h
    src/FileUtilities.cpp src/FileUtilities.h
    src/SystemCommand.cpp src/SystemCommand.h
    src/EventTimer.h
    src/Version.h
)

set (QSERIALTERMINAL_FORMS
        forms/MainWindow.ui
)

set (QSERIALTERMINAL_RESOURCES 
    resources/icons.qrc
)

qt5_wrap_ui (QSERIALTERMINAL_FORMS_MOC  ${QSERIALTERMINAL_FORMS})
qt5_add_resources(QSERIALTERMINAL_RESOURCES_RCC ${QSERIALTERMINAL_RESOURCES})

add_executable(QSerialTerminal "${QSERIALTERMINAL_SOURCES}"
                               "${QSERIALTERMINAL_HEADERS}"
                               "${QSERIALTERMINAL_FORMS}"
                               "${QSERIALTERMINAL_RESOURCES}")

if (win32)
    target_link_libraries (QSerialTerminal 
        Qt5::Widgets 
        Qt5::Gui 
        Qt5::Core 
    )
else()
    target_link_libraries (QSerialTerminal 
        Qt5::Widgets 
        Qt5::Gui 
        Qt5::Core 
        pthread
    )
endif()

