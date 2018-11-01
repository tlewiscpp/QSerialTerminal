cmake_minimum_required (VERSION 3.1)
project(QSerialTerminal CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set (THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third-party")

if (WIN32 OR WIN64)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set (DEPLOY_BUILD_ARGUMENT "--debug")
    else()
        set (DEPLOY_BUILD_ARGUMENT "--release")
    endif()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "-DNOMINMAX /EHsc /bigobj")
        set(COVERAGE_LINK_FLAGS  "/SUBSYSTEM:WINDOWS,5.01")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
        set(PLATFORM_SPECIFIC_LIBS Shlwapi getopt-win32)
    else()
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Weffc++")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
        set(COVERAGE_LINK_FLAGS  "-mwindows")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
        set(PLATFORM_SPECIFIC_LIBS pthread getopt-win32)
    endif()
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Weffc++ -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(COVERAGE_LINK_FLAGS  "")
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")

    set(PLATFORM_SPECIFIC_LIBS pthread dl)
endif()

set(CPP_SERIAL_PORT_SUBMODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third-party/CppSerialPort/")

if (EXISTS "${CPP_SERIAL_PORT_SUBMODULE_PATH}/CMakeLists.txt")
    message(STATUS "Using submodule CppSerialPort")
    add_subdirectory("${CPP_SERIAL_PORT_SUBMODULE_PATH}")
    unset(USE_SYSTEM_CPP_SERIAL_PORT)
    set(CPP_SERIAL_PORT_INCLUDE_DIR "${CPP_SERIAL_PORT_SUBMODULE_PATH}/include")
else()
    message(STATUS "Using system CppSerialPort")
    set(USE_SYSTEM_CPP_SERIAL_PORT TRUE)
    set(CPP_SERIAL_PORT_INCLUDE_DIR)
endif()


set(QT_PACKAGES Qt5Widgets Qt5Gui Qt5Core)
set(QT_LINK_LIBRARIES)
set(QT_LIBRARY_LIST)

foreach(Qt5Package ${QT_PACKAGES})
    set(Qt5PackageBase "")
    string(SUBSTRING ${Qt5Package} 0 3 Qt5PackageBase)
    string(SUBSTRING ${Qt5Package} 3 -1 Qt5PackageTail)
    string(CONCAT Qt5PackageLib "${Qt5PackageBase}" "::" "${Qt5PackageTail}")
    list(APPEND QT_LIBRARY_LIST "${Qt5PackageTail}")
    list(APPEND QT_LINK_LIBRARIES "${Qt5PackageLib}")
    if (WIN32 OR WIN64)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            set(${Qt5Package}_DIR "${QT_ROOT}/${QT_MSVC_VERSION}/lib/cmake/${Qt5Package}")
        else()
            set(${Qt5Package}_DIR "${QT_ROOT}/${QT_MINGW_VERSION}/lib/cmake/${Qt5Package}")
        endif()
        message(STATUS "${Qt5Package}_DIR = ${${Qt5Package}_DIR}")
    endif()
    find_package(${Qt5Package})
    if (${Qt5Package}_FOUND)
        message(STATUS "${Qt5Package}_INCLUDE_DIRS: ${${Qt5Package}_INCLUDE_DIRS}}")
        message(STATUS "${Qt5Package}_LIBRARIES: ${${Qt5Package}_LIBRARIES}")
        message(STATUS "${Qt5Package}_VERSION: ${${Qt5Package}_VERSION}")
        include_directories(${${Qt5Package}_INCLUDE_DIRS})
    endif()
endforeach(Qt5Package)


set(SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(FORMS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/forms")
set(RESOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(TRANSLATION_ROOT "${RESOURCE_ROOT}/translations")

set (${PROJECT_NAME}_SOURCE_FILES
        "${SOURCE_ROOT}/Main.cpp"
        "${SOURCE_ROOT}/ApplicationSettings.cpp"
        "${SOURCE_ROOT}/ApplicationSettingsLoader.cpp"
        "${SOURCE_ROOT}/MainWindow.cpp"
        "${SOURCE_ROOT}/ApplicationIcons.cpp"
        "${SOURCE_ROOT}/QSerialTerminalLineEdit.cpp"
        "${SOURCE_ROOT}/ApplicationUtilities.cpp"
        "${SOURCE_ROOT}/SingleInstanceGuard.cpp"
        "${SOURCE_ROOT}/AboutApplicationWidget.cpp")

set (${PROJECT_NAME}_HEADER_FILES
        "${SOURCE_ROOT}/GlobalDefinitions.hpp"
        "${SOURCE_ROOT}/ApplicationSettings.hpp"
        "${SOURCE_ROOT}/ApplicationSettingsLoader.hpp"
        "${SOURCE_ROOT}/MainWindow.hpp"
        "${SOURCE_ROOT}/ApplicationIcons.hpp"
        "${SOURCE_ROOT}/QSerialTerminalLineEdit.hpp"
        "${SOURCE_ROOT}/ApplicationUtilities.hpp"
        "${SOURCE_ROOT}/AboutApplicationWidget.hpp"
        "${SOURCE_ROOT}/SingleInstanceGuard.hpp"
        "${SOURCE_ROOT}/QActionSetDefs.hpp"
        "${SOURCE_ROOT}/ApplicationStrings.hpp"
        "${SOURCE_ROOT}/ProgramOption.hpp"
        "${SOURCE_ROOT}/Version.hpp")

set (${PROJECT_NAME}_FORMS
        "${FORMS_ROOT}/MainWindow.ui"
        "${FORMS_ROOT}/AboutApplicationWidget.ui")

set (${PROJECT_NAME}_RESOURCES
        "${RESOURCE_ROOT}/icons.qrc"
        "${RESOURCE_ROOT}/licenses.qrc")

set (${PROJECT_NAME}_TRANSLATION_TEMPLATES
        "${TRANSLATION_ROOT}/english.ts"
        "${TRANSLATIONS_ROO}/japanese.ts")


qt5_wrap_ui (${PROJECT_NAME}_FORMS_MOC  ${${PROJECT_NAME}_FORMS})

add_executable(${PROJECT_NAME}
        ${${PROJECT_NAME}_SOURCE_FILES}
        ${${PROJECT_NAME}_HEADER_FILES}
        ${${PROJECT_NAME}_RESOURCES}
        ${${PROJECT_NAME}_FORMS})

target_link_libraries (${PROJECT_NAME}
        ${PLATFORM_SPECIFIC_LIBS}
        ${QT_LINK_LIBRARIES}
        CppSerialPort)

if (WIN32 OR WIN64)
    if (WIN_COMPILER STREQUAL "MSVC")
        add_custom_command(TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND "${QT_ROOT}/${QT_MSVC_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/" "${DEPLOY_BUILD_ARGUMENT}")
    else()
        add_custom_command(TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND "${QT_ROOT}/${QT_MINGW_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/" "${DEPLOY_BUILD_ARGUMENT}")
    endif()
endif()

target_include_directories(${PROJECT_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PUBLIC "${SOURCE_ROOT}"
        PUBLIC "${INCLUDE_ROOT}"
        PUBLIC "${CPP_SERIAL_PORT_INCLUDE_DIR}"
        PUBLIC "${GETOPT_INCLUDE_DIR}")

set(DESKTOP_FILE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/utility/${PROJECT_NAME}.desktop")
set(CONFIGURATION_FILE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/utility/${PROJECT_NAME}.conf")
set(ICON_FILE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/resources/${PROJECT_NAME}.png")

if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    install(DIRECTORY DESTINATION "/etc/${PROJECT_NAME}/")
    install(FILES "${CONFIGURATION_FILE_LOCATION}" DESTINATION "/etc/${PROJECT_NAME}/")
    install(DIRECTORY DESTINATION "$ENV{HOME}/Desktop")
    install(FILES "${DESKTOP_FILE_LOCATION}" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
    install(FILES "${DESKTOP_FILE_LOCATION}" DESTINATION "$ENV{HOME}/Desktop")
    install(FILES "${ICON_FILE_LOCATION}" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixmaps")
endif()
