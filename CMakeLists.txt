cmake_minimum_required (VERSION 3.1)
set (PROJECT_NAME QSerialTerminal)
project(${PROJECT_NAME} CXX)

set(CMAKE_BUILD_TYPE Debug)
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

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(WIN_COMPILER "MSVC")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(WIN_COMPILER "MINGW")
	endif()
    if (NOT WIN_COMPILER)
        message(FATAL_ERROR "Please specify -DWIN_COMPILER=MSVC or -DWIN_COMPILER=MINGW")
    endif()
    IF (WIN_COMPILER STREQUAL "MINGW")
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
        set(CMAKE_CXX_FLAGS_DEBUG "-g")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()
        if (WIN_COMPILER STREQUAL "MSVC")
        set (WINDOWS_COMPILER MSVC)
        set(COVERAGE_LINK_FLAGS  "/SUBSYSTEM:WINDOWS ")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    else()
        set (WINDOWS_COMPILER MINGW)
        set(COVERAGE_LINK_FLAGS  "-mwindows")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    endif()


    #Change this variable to the current Qt root!
    if (NOT QT_ROOT)
        set(QT_ROOT "C:/Qt/5.10.0")
    endif()
    #Change this variable to the mingw version used by your Qt build version (NOT THE COMPILER PATH)
    if (WIN_COMPILER STREQUAL "MSVC")
        if (NOT QT_MSVC_VERSION)
            set (QT_MSVC_VERSION "msvc2017_64")
        endif()
    else()
        if (NOT QT_MINGW_VERSION)
            set(QT_MINGW_VERSION "mingw53_32")
        endif()
    endif()
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()


set(QT_PACKAGES Qt5Widgets Qt5Gui Qt5Core Qt5Multimedia Qt5SerialPort)
set(QT_LINK_LIBRARIES)
set(QT_LIBRARY_LIST)

if (WIN32)
        if (WIN_COMPILER STREQUAL "MSVC")
            set(Qt5_DIR "${QT_ROOT}/${QT_MSVC_VERSION}/lib/cmake/Qt5")
            set(QT_QMAKE_EXECUTABLE "${QT_ROOT}/${QT_MSVC_VERSION}/bin/qmake.exe")
            message(STATUS "QT_ROOT = ${QT_ROOT}")
        else()
            set(Qt5_DIR "${QT_ROOT}/${QT_MINGW_VERSION}/lib/cmake/Qt5")
            set(QT_QMAKE_EXECUTABLE "${QT_ROOT}/${QT_MINGW_VERSION}/bin/qmake.exe")
            message(STATUS "QT_ROOT = ${QT_ROOT}")
        endif()
endif()

foreach(Qt5Package ${QT_PACKAGES})
    set(Qt5PackageBase "")
    string(SUBSTRING ${Qt5Package} 0 3 Qt5PackageBase)
    string(SUBSTRING ${Qt5Package} 3 -1 Qt5PackageTail)
    string(CONCAT Qt5PackageLib "${Qt5PackageBase}" "::" "${Qt5PackageTail}")
    list(APPEND QT_LIBRARY_LIST "${Qt5PackageTail}")
    list(APPEND QT_LINK_LIBRARIES "${Qt5PackageLib}")
    if (WIN32)
        if (WIN_COMPILER STREQUAL "MSVC")
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


set (SOURCE_ROOT src)

set (${PROJECT_NAME}_SOURCE_FILES
        ${SOURCE_ROOT}/ByteArray.cpp
        ${SOURCE_ROOT}/Main.cpp
        ${SOURCE_ROOT}/ApplicationSettings.cpp
        ${SOURCE_ROOT}/ApplicationSettingsLoader.cpp
        ${SOURCE_ROOT}/MainWindow.cpp
        ${SOURCE_ROOT}/ApplicationIcons.cpp
        ${SOURCE_ROOT}/QSerialTerminalLineEdit.cpp
        ${SOURCE_ROOT}/ApplicationUtilities.cpp
        ${SOURCE_ROOT}/SerialPort.cpp
        ${SOURCE_ROOT}/IByteStream.cpp
        ${SOURCE_ROOT}/SingleInstanceGuard.cpp
        ${SOURCE_ROOT}/AboutApplicationWidget.cpp)

set (${PROJECT_NAME}_HEADER_FILES
        ${SOURCE_ROOT}/ByteArray.h
        ${SOURCE_ROOT}/GlobalDefinitions.h
        ${SOURCE_ROOT}/ApplicationSettings.h
        ${SOURCE_ROOT}/ApplicationSettingsLoader.h
        ${SOURCE_ROOT}/MainWindow.h
        ${SOURCE_ROOT}/ApplicationIcons.h
        ${SOURCE_ROOT}/QSerialTerminalLineEdit.h
        ${SOURCE_ROOT}/ApplicationUtilities.h
        ${SOURCE_ROOT}/SerialPort.h
        ${SOURCE_ROOT}/IByteStream.h
        ${SOURCE_ROOT}/AboutApplicationWidget.h
        ${SOURCE_ROOT}/SingleInstanceGuard.h
        ${SOURCE_ROOT}/QActionSetDefs.h
        ${SOURCE_ROOT}/ApplicationStrings.h
        ${SOURCE_ROOT}/Version.h)

set (${PROJECT_NAME}_FORMS
        forms/MainWindow.ui
        forms/AboutApplicationWidget.ui)

set (${PROJECT_NAME}_RESOURCES 
        resources/icons.qrc
        resources/licenses.qrc)

set (${PROJECT_NAME}_TRANSLATION_TEMPLATES
    resources/translations/english.ts
    resources/translations/japanese.ts)

qt5_wrap_ui (${PROJECT_NAME}_FORMS_MOC  ${${PROJECT_NAME}_FORMS})

if (WIN32)
	add_executable(${PROJECT_NAME}
			WIN32
			${${PROJECT_NAME}_SOURCE_FILES}
			${${PROJECT_NAME}_HEADER_FILES}
			${${PROJECT_NAME}_FORMS}
			${${PROJECT_NAME}_RESOURCES_RCC}
			resources/${PROJECT_NAME}.rc)


	if (WIN_COMPILER STREQUAL "MSVC")
		add_custom_command(TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND "${QT_ROOT}/${QT_MSVC_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/")
	elseif(WIN_COMPILER STREQUAL "MINGW")
		add_custom_command(TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND "${QT_ROOT}/${QT_MINGW_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/")

	endif()

	target_link_libraries(${PROJECT_NAME} Shlwapi)
else()
	add_executable(${PROJECT_NAME}
			${${PROJECT_NAME}_SOURCE_FILES}
			${${PROJECT_NAME}_HEADER_FILES}
			${${PROJECT_NAME}_FORMS}
			${${PROJECT_NAME}_RESOURCES_RCC})

	target_link_libraries (${PROJECT_NAME}
			pthread)
endif()

qt5_use_modules(${PROJECT_NAME} ${QT_LIBRARY_LIST})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

if (WIN32)
else()
    target_link_libraries(${PROJECT_NAME} pthread)
endif()

if (WIN32)
	if (WIN_COMPILER STREQUAL "MSVC")
		add_custom_command(TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND "${QT_ROOT}/${QT_MSVC_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/")
	elseif(WIN_COMPILER STREQUAL "MINGW")
		add_custom_command(TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND "${QT_ROOT}/${QT_MINGW_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/")

	endif()
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    install(DIRECTORY DESTINATION "$ENV{HOME}/Desktop")
    install(FILES "${DESKTOP_FILE_LOCATION}" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
    install(FILES "${DESKTOP_FILE_LOCATION}" DESTINATION "$ENV{HOME}/Desktop")
    install(FILES "${ICON_FILE_LOCATION}" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixmaps")
endif()

