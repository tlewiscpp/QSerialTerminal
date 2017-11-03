cmake_minimum_required (VERSION 3.1)
set (PROJECT_NAME QSerialTerminal)
project(${PROJECT_NAME} CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(src/)

set (${PROJECT_NAME}_SOURCES 
    src/Main.cpp
    src/GlobalDefinitions.h
    src/ApplicationSettings.cpp src/ApplicationSettings.h
    src/ApplicationSettingsLoader.cpp src/ApplicationSettingsLoader.h
    src/MainWindow.cpp src/MainWindow.h
    src/ApplicationIcons.cpp src/ApplicationIcons.h
        src/ApplicationStrings.h
        src/QSerialTerminalLineEdit.cpp src/QSerialTerminalLineEdit.h
    src/ApplicationUtilities.cpp src/ApplicationUtilities.h
    src/SerialPort.cpp src/SerialPort.h
    src/IByteStream.cpp src/IByteStream.h
    src/AboutApplicationWidget.cpp src/AboutApplicationWidget.h
    src/Version.h
)

set (${PROJECT_NAME}_FORMS
        forms/MainWindow.ui
        forms/AboutApplicationWidget.ui
)

set (${PROJECT_NAME}_RESOURCES 
    resources/icons.qrc
        resources/licenses.qrc
)

set (${PROJECT_NAME}_TRANSLATION_TEMPLATES
    resources/translations/english.ts
    resources/translations/japanese.ts
        )

qt5_wrap_cpp(${PROJECT_NAME}_HEADERS_MOC ${${PROJECT_NAME}_HEADERS})
qt5_wrap_ui (${PROJECT_NAME}_FORMS_MOC  ${${PROJECT_NAME}_FORMS})
#qt5_add_translation(${PROJECT_NAME}_TRANSLATIONS ${TRMIDOUBLECHECK_TRANSLATION_TEMPLATES})

add_executable(${PROJECT_NAME}
        ${${PROJECT_NAME}_SOURCES}
        ${${PROJECT_NAME}_FORMS}
        ${${PROJECT_NAME}_RESOURCES}
        )

if (win32)
    target_link_libraries (${PROJECT_NAME}
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

