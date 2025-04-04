# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build

# Include any dependencies generated for this target.
include src/console/CMakeFiles/console_module.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/console/CMakeFiles/console_module.dir/compiler_depend.make

# Include the progress variables for this target.
include src/console/CMakeFiles/console_module.dir/progress.make

# Include the compile flags for this target's objects.
include src/console/CMakeFiles/console_module.dir/flags.make

src/console/CMakeFiles/console_module.dir/codegen:
.PHONY : src/console/CMakeFiles/console_module.dir/codegen

src/console/CMakeFiles/console_module.dir/console_display.cpp.o: src/console/CMakeFiles/console_module.dir/flags.make
src/console/CMakeFiles/console_module.dir/console_display.cpp.o: /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/src/console/console_display.cpp
src/console/CMakeFiles/console_module.dir/console_display.cpp.o: src/console/CMakeFiles/console_module.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/console/CMakeFiles/console_module.dir/console_display.cpp.o"
	cd /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console && /Users/danny/k/apps/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/console/CMakeFiles/console_module.dir/console_display.cpp.o -MF CMakeFiles/console_module.dir/console_display.cpp.o.d -o CMakeFiles/console_module.dir/console_display.cpp.o -c /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/src/console/console_display.cpp

src/console/CMakeFiles/console_module.dir/console_display.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/console_module.dir/console_display.cpp.i"
	cd /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console && /Users/danny/k/apps/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/src/console/console_display.cpp > CMakeFiles/console_module.dir/console_display.cpp.i

src/console/CMakeFiles/console_module.dir/console_display.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/console_module.dir/console_display.cpp.s"
	cd /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console && /Users/danny/k/apps/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/src/console/console_display.cpp -o CMakeFiles/console_module.dir/console_display.cpp.s

# Object files for target console_module
console_module_OBJECTS = \
"CMakeFiles/console_module.dir/console_display.cpp.o"

# External object files for target console_module
console_module_EXTERNAL_OBJECTS =

src/console/libconsole_module.a: src/console/CMakeFiles/console_module.dir/console_display.cpp.o
src/console/libconsole_module.a: src/console/CMakeFiles/console_module.dir/build.make
src/console/libconsole_module.a: src/console/CMakeFiles/console_module.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libconsole_module.a"
	cd /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console && $(CMAKE_COMMAND) -P CMakeFiles/console_module.dir/cmake_clean_target.cmake
	cd /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/console_module.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/console/CMakeFiles/console_module.dir/build: src/console/libconsole_module.a
.PHONY : src/console/CMakeFiles/console_module.dir/build

src/console/CMakeFiles/console_module.dir/clean:
	cd /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console && $(CMAKE_COMMAND) -P CMakeFiles/console_module.dir/cmake_clean.cmake
.PHONY : src/console/CMakeFiles/console_module.dir/clean

src/console/CMakeFiles/console_module.dir/depend:
	cd /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/src/console /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console /Users/danny/k/codes/snort3_1/self-packet-tracker/tcp_trakcer/build/src/console/CMakeFiles/console_module.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/console/CMakeFiles/console_module.dir/depend

