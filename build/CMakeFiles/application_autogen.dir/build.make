# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\cmake-3.28.1-windows-x86_64\bin\cmake.exe

# The command to remove a file.
RM = C:\cmake-3.28.1-windows-x86_64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot\build"

# Utility rule file for application_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/application_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/application_autogen.dir/progress.make

CMakeFiles/application_autogen: application_autogen/timestamp

application_autogen/timestamp: C:/Qt/5.15.2/mingw81_64/bin/moc.exe
application_autogen/timestamp: C:/Qt/5.15.2/mingw81_64/bin/uic.exe
application_autogen/timestamp: CMakeFiles/application_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target application"
	C:\cmake-3.28.1-windows-x86_64\bin\cmake.exe -E cmake_autogen "C:/Users/Steven Tu/Documents/vs_code_workspace/wifi-controlled-robot/build/CMakeFiles/application_autogen.dir/AutogenInfo.json" Debug
	C:\cmake-3.28.1-windows-x86_64\bin\cmake.exe -E touch "C:/Users/Steven Tu/Documents/vs_code_workspace/wifi-controlled-robot/build/application_autogen/timestamp"

application_autogen: CMakeFiles/application_autogen
application_autogen: application_autogen/timestamp
application_autogen: CMakeFiles/application_autogen.dir/build.make
.PHONY : application_autogen

# Rule to build all files generated by this target.
CMakeFiles/application_autogen.dir/build: application_autogen
.PHONY : CMakeFiles/application_autogen.dir/build

CMakeFiles/application_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\application_autogen.dir\cmake_clean.cmake
.PHONY : CMakeFiles/application_autogen.dir/clean

CMakeFiles/application_autogen.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot" "C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot" "C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot\build" "C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot\build" "C:\Users\Steven Tu\Documents\vs_code_workspace\wifi-controlled-robot\build\CMakeFiles\application_autogen.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/application_autogen.dir/depend

