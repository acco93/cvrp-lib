# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /home/acco/Downloads/CLion-2019.2/clion-2019.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/acco/Downloads/CLion-2019.2/clion-2019.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/acco/CLionProjects/cvrp-lib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/acco/CLionProjects/cvrp-lib/cmake-build-debug

# Include any dependencies generated for this target.
include vrp-instance/test/CMakeFiles/vrp-instance-test.dir/depend.make

# Include the progress variables for this target.
include vrp-instance/test/CMakeFiles/vrp-instance-test.dir/progress.make

# Include the compile flags for this target's objects.
include vrp-instance/test/CMakeFiles/vrp-instance-test.dir/flags.make

vrp-instance/test/CMakeFiles/vrp-instance-test.dir/main.cpp.o: vrp-instance/test/CMakeFiles/vrp-instance-test.dir/flags.make
vrp-instance/test/CMakeFiles/vrp-instance-test.dir/main.cpp.o: ../vrp-instance/test/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/acco/CLionProjects/cvrp-lib/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object vrp-instance/test/CMakeFiles/vrp-instance-test.dir/main.cpp.o"
	cd /home/acco/CLionProjects/cvrp-lib/cmake-build-debug/vrp-instance/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vrp-instance-test.dir/main.cpp.o -c /home/acco/CLionProjects/cvrp-lib/vrp-instance/test/main.cpp

vrp-instance/test/CMakeFiles/vrp-instance-test.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vrp-instance-test.dir/main.cpp.i"
	cd /home/acco/CLionProjects/cvrp-lib/cmake-build-debug/vrp-instance/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/acco/CLionProjects/cvrp-lib/vrp-instance/test/main.cpp > CMakeFiles/vrp-instance-test.dir/main.cpp.i

vrp-instance/test/CMakeFiles/vrp-instance-test.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vrp-instance-test.dir/main.cpp.s"
	cd /home/acco/CLionProjects/cvrp-lib/cmake-build-debug/vrp-instance/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/acco/CLionProjects/cvrp-lib/vrp-instance/test/main.cpp -o CMakeFiles/vrp-instance-test.dir/main.cpp.s

# Object files for target vrp-instance-test
vrp__instance__test_OBJECTS = \
"CMakeFiles/vrp-instance-test.dir/main.cpp.o"

# External object files for target vrp-instance-test
vrp__instance__test_EXTERNAL_OBJECTS =

vrp-instance/test/vrp-instance-test: vrp-instance/test/CMakeFiles/vrp-instance-test.dir/main.cpp.o
vrp-instance/test/vrp-instance-test: vrp-instance/test/CMakeFiles/vrp-instance-test.dir/build.make
vrp-instance/test/vrp-instance-test: vrp-instance/src/libvrp-instance-lib.a
vrp-instance/test/vrp-instance-test: googletest-master/googlemock/gtest/libgtestd.a
vrp-instance/test/vrp-instance-test: googletest-master/googlemock/gtest/libgtest_maind.a
vrp-instance/test/vrp-instance-test: raw-mm/src/libraw-mm-lib.a
vrp-instance/test/vrp-instance-test: googletest-master/googlemock/gtest/libgtestd.a
vrp-instance/test/vrp-instance-test: vrp-instance/test/CMakeFiles/vrp-instance-test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/acco/CLionProjects/cvrp-lib/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable vrp-instance-test"
	cd /home/acco/CLionProjects/cvrp-lib/cmake-build-debug/vrp-instance/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vrp-instance-test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
vrp-instance/test/CMakeFiles/vrp-instance-test.dir/build: vrp-instance/test/vrp-instance-test

.PHONY : vrp-instance/test/CMakeFiles/vrp-instance-test.dir/build

vrp-instance/test/CMakeFiles/vrp-instance-test.dir/clean:
	cd /home/acco/CLionProjects/cvrp-lib/cmake-build-debug/vrp-instance/test && $(CMAKE_COMMAND) -P CMakeFiles/vrp-instance-test.dir/cmake_clean.cmake
.PHONY : vrp-instance/test/CMakeFiles/vrp-instance-test.dir/clean

vrp-instance/test/CMakeFiles/vrp-instance-test.dir/depend:
	cd /home/acco/CLionProjects/cvrp-lib/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/acco/CLionProjects/cvrp-lib /home/acco/CLionProjects/cvrp-lib/vrp-instance/test /home/acco/CLionProjects/cvrp-lib/cmake-build-debug /home/acco/CLionProjects/cvrp-lib/cmake-build-debug/vrp-instance/test /home/acco/CLionProjects/cvrp-lib/cmake-build-debug/vrp-instance/test/CMakeFiles/vrp-instance-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : vrp-instance/test/CMakeFiles/vrp-instance-test.dir/depend

