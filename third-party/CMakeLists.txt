cmake_minimum_required(VERSION 3.1)
set (PROJECT_NAME QSerialTerminal)

if (WIN32 OR WIN64)
    add_subdirectory(win32)
endif()

add_subdirectory(CppSerialPort)