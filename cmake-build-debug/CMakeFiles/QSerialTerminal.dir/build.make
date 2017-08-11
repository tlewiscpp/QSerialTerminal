# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pinguinsan/GitHub/QSerialTerminal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/QSerialTerminal.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/QSerialTerminal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/QSerialTerminal.dir/flags.make

ui_MainWindow.h: ../forms/MainWindow.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ui_MainWindow.h"
	/usr/bin/uic -o /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/ui_MainWindow.h /home/pinguinsan/GitHub/QSerialTerminal/forms/MainWindow.ui

ui_AboutApplicationWidget.h: ../forms/AboutApplicationWidget.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating ui_AboutApplicationWidget.h"
	/usr/bin/uic -o /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/ui_AboutApplicationWidget.h /home/pinguinsan/GitHub/QSerialTerminal/forms/AboutApplicationWidget.ui

qrc_icons.cpp: ../resources/icons/qserialterminal.png
qrc_icons.cpp: ../resources/icons.qrc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating qrc_icons.cpp"
	/usr/bin/rcc --name icons --output /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/qrc_icons.cpp /home/pinguinsan/GitHub/QSerialTerminal/resources/icons.qrc

qrc_licenses.cpp: ../resources/licenses/LICENSE
qrc_licenses.cpp: ../resources/licenses.qrc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating qrc_licenses.cpp"
	/usr/bin/rcc --name licenses --output /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/qrc_licenses.cpp /home/pinguinsan/GitHub/QSerialTerminal/resources/licenses.qrc

CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o: ../src/Main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/Main.cpp

CMakeFiles/QSerialTerminal.dir/src/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/Main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/Main.cpp > CMakeFiles/QSerialTerminal.dir/src/Main.cpp.i

CMakeFiles/QSerialTerminal.dir/src/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/Main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/Main.cpp -o CMakeFiles/QSerialTerminal.dir/src/Main.cpp.s

CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o


CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o: ../src/ApplicationSettings.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationSettings.cpp

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationSettings.cpp > CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.i

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationSettings.cpp -o CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.s

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o


CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o: ../src/ApplicationSettingsLoader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationSettingsLoader.cpp

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationSettingsLoader.cpp > CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.i

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationSettingsLoader.cpp -o CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.s

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o


CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o: ../src/MainWindow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/MainWindow.cpp

CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/MainWindow.cpp > CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.i

CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/MainWindow.cpp -o CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.s

CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o


CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o: ../src/ApplicationIcons.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationIcons.cpp

CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationIcons.cpp > CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.i

CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationIcons.cpp -o CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.s

CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o


CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o: ../src/ApplicationStrings.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationStrings.cpp

CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationStrings.cpp > CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.i

CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationStrings.cpp -o CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.s

CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o


CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o: ../src/CustomAction.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/CustomAction.cpp

CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/CustomAction.cpp > CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.i

CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/CustomAction.cpp -o CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.s

CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o


CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o: ../src/QSerialTerminalLineEdit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/QSerialTerminalLineEdit.cpp

CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/QSerialTerminalLineEdit.cpp > CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.i

CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/QSerialTerminalLineEdit.cpp -o CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.s

CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o


CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o: ../src/ApplicationUtilities.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationUtilities.cpp

CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationUtilities.cpp > CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.i

CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/ApplicationUtilities.cpp -o CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.s

CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o


CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o: ../src/SerialPort.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/SerialPort.cpp

CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/SerialPort.cpp > CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.i

CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/SerialPort.cpp -o CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.s

CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o


CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o: ../src/IByteStream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/IByteStream.cpp

CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/IByteStream.cpp > CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.i

CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/IByteStream.cpp -o CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.s

CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o


CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o: ../src/AboutApplicationWidget.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/src/AboutApplicationWidget.cpp

CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/src/AboutApplicationWidget.cpp > CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.i

CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/src/AboutApplicationWidget.cpp -o CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.s

CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o


CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o: CMakeFiles/QSerialTerminal.dir/flags.make
CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o: QSerialTerminal_autogen/moc_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o -c /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/QSerialTerminal_autogen/moc_compilation.cpp

CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/QSerialTerminal_autogen/moc_compilation.cpp > CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.i

CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/QSerialTerminal_autogen/moc_compilation.cpp -o CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.s

CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.requires:

.PHONY : CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.requires

CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.provides: CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.requires
	$(MAKE) -f CMakeFiles/QSerialTerminal.dir/build.make CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.provides.build
.PHONY : CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.provides

CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.provides.build: CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o


# Object files for target QSerialTerminal
QSerialTerminal_OBJECTS = \
"CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o" \
"CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o"

# External object files for target QSerialTerminal
QSerialTerminal_EXTERNAL_OBJECTS =

QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/build.make
QSerialTerminal: /usr/lib/libQt5Widgets.so.5.9.1
QSerialTerminal: /usr/lib/libQt5Gui.so.5.9.1
QSerialTerminal: /usr/lib/libQt5Core.so.5.9.1
QSerialTerminal: CMakeFiles/QSerialTerminal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Linking CXX executable QSerialTerminal"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QSerialTerminal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/QSerialTerminal.dir/build: QSerialTerminal

.PHONY : CMakeFiles/QSerialTerminal.dir/build

CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/Main.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettings.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/ApplicationSettingsLoader.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/MainWindow.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/ApplicationIcons.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/ApplicationStrings.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/CustomAction.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/QSerialTerminalLineEdit.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/ApplicationUtilities.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/SerialPort.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/IByteStream.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/src/AboutApplicationWidget.cpp.o.requires
CMakeFiles/QSerialTerminal.dir/requires: CMakeFiles/QSerialTerminal.dir/QSerialTerminal_autogen/moc_compilation.cpp.o.requires

.PHONY : CMakeFiles/QSerialTerminal.dir/requires

CMakeFiles/QSerialTerminal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/QSerialTerminal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/QSerialTerminal.dir/clean

CMakeFiles/QSerialTerminal.dir/depend: ui_MainWindow.h
CMakeFiles/QSerialTerminal.dir/depend: ui_AboutApplicationWidget.h
CMakeFiles/QSerialTerminal.dir/depend: qrc_icons.cpp
CMakeFiles/QSerialTerminal.dir/depend: qrc_licenses.cpp
	cd /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pinguinsan/GitHub/QSerialTerminal /home/pinguinsan/GitHub/QSerialTerminal /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug /home/pinguinsan/GitHub/QSerialTerminal/cmake-build-debug/CMakeFiles/QSerialTerminal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/QSerialTerminal.dir/depend
