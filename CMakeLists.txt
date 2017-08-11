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
        src/GlobalDefinitions.h
        src/ApplicationSettings.cpp src/ApplicationSettings.h
        src/ApplicationSettingsLoader.cpp src/ApplicationSettingsLoader.h
    src/MainWindow.cpp src/MainWindow.h
    src/ApplicationIcons.cpp src/ApplicationIcons.h
    src/ApplicationStrings.cpp src/ApplicationStrings.h
    src/CustomAction.cpp src/CustomAction.h
    src/QSerialTerminalLineEdit.cpp src/QSerialTerminalLineEdit.h
    src/ApplicationUtilities.cpp src/ApplicationUtilities.h
    src/SerialPort.cpp src/SerialPort.h
    src/IByteStream.cpp src/IByteStream.h
        src/AboutApplicationWidget.cpp src/AboutApplicationWidget.h
    src/EventTimer.h
    src/Version.h
)

set (QSERIALTERMINAL_FORMS
        forms/MainWindow.ui
        forms/AboutApplicationWidget.ui
)

set (QSERIALTERMINAL_RESOURCES 
    resources/icons.qrc
        resources/licenses.qrc
)

qt5_wrap_ui (QSERIALTERMINAL_FORMS_MOC  ${QSERIALTERMINAL_FORMS})
qt5_add_resources(QSERIALTERMINAL_RESOURCES_RCC ${QSERIALTERMINAL_RESOURCES})

add_executable(${PROJECT_NAME}
        ${QSERIALTERMINAL_SOURCES}
        ${QSERIALTERMINAL_HEADERS}
        ${QSERIALTERMINAL_FORMS}
        ${QSERIALTERMINAL_RESOURCES}
        )

if (win32)
    target_link_libraries (QSerialTerminal 
        Qt5::Widgets 
        Qt5::Gui 
        Qt5::Core 
    )
else()
    target_link_libraries (${PROJECT_NAME}
        Qt5::Widgets 
        Qt5::Gui 
        Qt5::Core 
        pthread
    )
endif()

