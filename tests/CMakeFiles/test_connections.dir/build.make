# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/developer/ducttape-engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/developer/ducttape-engine

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_connections.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_connections.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_connections.dir/flags.make

tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o: tests/CMakeFiles/test_connections.dir/flags.make
tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o: tests/src/test_connections.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/developer/ducttape-engine/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o"
	cd /home/developer/ducttape-engine/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test_connections.dir/src/test_connections.cpp.o -c /home/developer/ducttape-engine/tests/src/test_connections.cpp

tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_connections.dir/src/test_connections.cpp.i"
	cd /home/developer/ducttape-engine/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/developer/ducttape-engine/tests/src/test_connections.cpp > CMakeFiles/test_connections.dir/src/test_connections.cpp.i

tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_connections.dir/src/test_connections.cpp.s"
	cd /home/developer/ducttape-engine/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/developer/ducttape-engine/tests/src/test_connections.cpp -o CMakeFiles/test_connections.dir/src/test_connections.cpp.s

tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.requires:
.PHONY : tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.requires

tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.provides: tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/test_connections.dir/build.make tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.provides.build
.PHONY : tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.provides

tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.provides.build: tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o

# Object files for target test_connections
test_connections_OBJECTS = \
"CMakeFiles/test_connections.dir/src/test_connections.cpp.o"

# External object files for target test_connections
test_connections_EXTERNAL_OBJECTS =

bin/test_connections: tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o
bin/test_connections: lib/libducttape.so.0.1.0
bin/test_connections: /usr/lib/libboost_filesystem.so
bin/test_connections: /usr/lib/libboost_program_options.so
bin/test_connections: /usr/lib/libboost_system.so
bin/test_connections: /usr/lib/libboost_date_time.so
bin/test_connections: /usr/lib/libboost_thread.so
bin/test_connections: /usr/lib/libsfml-audio.so
bin/test_connections: /usr/lib/libsfml-graphics.so
bin/test_connections: /usr/lib/libsfml-network.so
bin/test_connections: /usr/lib/libsfml-system.so
bin/test_connections: /usr/lib/libsfml-window.so
bin/test_connections: /usr/lib/libBulletDynamics.so
bin/test_connections: /usr/lib/libBulletCollision.so
bin/test_connections: /usr/lib/libLinearMath.so
bin/test_connections: /usr/lib/libBulletSoftBody.so
bin/test_connections: /usr/lib/libOgreMain.so
bin/test_connections: /usr/lib/libOIS.so
bin/test_connections: tests/CMakeFiles/test_connections.dir/build.make
bin/test_connections: tests/CMakeFiles/test_connections.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/test_connections"
	cd /home/developer/ducttape-engine/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_connections.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_connections.dir/build: bin/test_connections
.PHONY : tests/CMakeFiles/test_connections.dir/build

tests/CMakeFiles/test_connections.dir/requires: tests/CMakeFiles/test_connections.dir/src/test_connections.cpp.o.requires
.PHONY : tests/CMakeFiles/test_connections.dir/requires

tests/CMakeFiles/test_connections.dir/clean:
	cd /home/developer/ducttape-engine/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_connections.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_connections.dir/clean

tests/CMakeFiles/test_connections.dir/depend:
	cd /home/developer/ducttape-engine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/developer/ducttape-engine /home/developer/ducttape-engine/tests /home/developer/ducttape-engine /home/developer/ducttape-engine/tests /home/developer/ducttape-engine/tests/CMakeFiles/test_connections.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_connections.dir/depend

