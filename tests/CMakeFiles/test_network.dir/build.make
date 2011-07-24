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
include tests/CMakeFiles/test_network.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_network.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_network.dir/flags.make

tests/CMakeFiles/test_network.dir/src/test_network.cpp.o: tests/CMakeFiles/test_network.dir/flags.make
tests/CMakeFiles/test_network.dir/src/test_network.cpp.o: tests/src/test_network.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/developer/ducttape-engine/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/CMakeFiles/test_network.dir/src/test_network.cpp.o"
	cd /home/developer/ducttape-engine/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test_network.dir/src/test_network.cpp.o -c /home/developer/ducttape-engine/tests/src/test_network.cpp

tests/CMakeFiles/test_network.dir/src/test_network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_network.dir/src/test_network.cpp.i"
	cd /home/developer/ducttape-engine/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/developer/ducttape-engine/tests/src/test_network.cpp > CMakeFiles/test_network.dir/src/test_network.cpp.i

tests/CMakeFiles/test_network.dir/src/test_network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_network.dir/src/test_network.cpp.s"
	cd /home/developer/ducttape-engine/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/developer/ducttape-engine/tests/src/test_network.cpp -o CMakeFiles/test_network.dir/src/test_network.cpp.s

tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.requires:
.PHONY : tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.requires

tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.provides: tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/test_network.dir/build.make tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.provides.build
.PHONY : tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.provides

tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.provides.build: tests/CMakeFiles/test_network.dir/src/test_network.cpp.o

# Object files for target test_network
test_network_OBJECTS = \
"CMakeFiles/test_network.dir/src/test_network.cpp.o"

# External object files for target test_network
test_network_EXTERNAL_OBJECTS =

bin/test_network: tests/CMakeFiles/test_network.dir/src/test_network.cpp.o
bin/test_network: lib/libducttape.so.0.1.0
bin/test_network: /usr/lib/libboost_filesystem.so
bin/test_network: /usr/lib/libboost_program_options.so
bin/test_network: /usr/lib/libboost_system.so
bin/test_network: /usr/lib/libboost_date_time.so
bin/test_network: /usr/lib/libboost_thread.so
bin/test_network: /usr/lib/libsfml-audio.so
bin/test_network: /usr/lib/libsfml-graphics.so
bin/test_network: /usr/lib/libsfml-network.so
bin/test_network: /usr/lib/libsfml-system.so
bin/test_network: /usr/lib/libsfml-window.so
bin/test_network: /usr/lib/libBulletDynamics.so
bin/test_network: /usr/lib/libBulletCollision.so
bin/test_network: /usr/lib/libLinearMath.so
bin/test_network: /usr/lib/libBulletSoftBody.so
bin/test_network: /usr/lib/libOgreMain.so
bin/test_network: /usr/lib/libOIS.so
bin/test_network: tests/CMakeFiles/test_network.dir/build.make
bin/test_network: tests/CMakeFiles/test_network.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/test_network"
	cd /home/developer/ducttape-engine/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_network.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_network.dir/build: bin/test_network
.PHONY : tests/CMakeFiles/test_network.dir/build

tests/CMakeFiles/test_network.dir/requires: tests/CMakeFiles/test_network.dir/src/test_network.cpp.o.requires
.PHONY : tests/CMakeFiles/test_network.dir/requires

tests/CMakeFiles/test_network.dir/clean:
	cd /home/developer/ducttape-engine/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_network.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_network.dir/clean

tests/CMakeFiles/test_network.dir/depend:
	cd /home/developer/ducttape-engine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/developer/ducttape-engine /home/developer/ducttape-engine/tests /home/developer/ducttape-engine /home/developer/ducttape-engine/tests /home/developer/ducttape-engine/tests/CMakeFiles/test_network.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_network.dir/depend

