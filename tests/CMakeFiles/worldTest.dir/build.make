# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alex/mrdevgit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alex/mrdevgit

# Include any dependencies generated for this target.
include tests/CMakeFiles/worldTest.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/worldTest.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/worldTest.dir/flags.make

tests/CMakeFiles/worldTest.dir/worldTest.cpp.o: tests/CMakeFiles/worldTest.dir/flags.make
tests/CMakeFiles/worldTest.dir/worldTest.cpp.o: tests/worldTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/alex/mrdevgit/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/CMakeFiles/worldTest.dir/worldTest.cpp.o"
	cd /home/alex/mrdevgit/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/worldTest.dir/worldTest.cpp.o -c /home/alex/mrdevgit/tests/worldTest.cpp

tests/CMakeFiles/worldTest.dir/worldTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/worldTest.dir/worldTest.cpp.i"
	cd /home/alex/mrdevgit/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/alex/mrdevgit/tests/worldTest.cpp > CMakeFiles/worldTest.dir/worldTest.cpp.i

tests/CMakeFiles/worldTest.dir/worldTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/worldTest.dir/worldTest.cpp.s"
	cd /home/alex/mrdevgit/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/alex/mrdevgit/tests/worldTest.cpp -o CMakeFiles/worldTest.dir/worldTest.cpp.s

tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.requires:
.PHONY : tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.requires

tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.provides: tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/worldTest.dir/build.make tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.provides.build
.PHONY : tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.provides

tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.provides.build: tests/CMakeFiles/worldTest.dir/worldTest.cpp.o

# Object files for target worldTest
worldTest_OBJECTS = \
"CMakeFiles/worldTest.dir/worldTest.cpp.o"

# External object files for target worldTest
worldTest_EXTERNAL_OBJECTS =

tests/worldTest: tests/CMakeFiles/worldTest.dir/worldTest.cpp.o
tests/worldTest: tests/CMakeFiles/worldTest.dir/build.make
tests/worldTest: tests/CMakeFiles/worldTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable worldTest"
	cd /home/alex/mrdevgit/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/worldTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/worldTest.dir/build: tests/worldTest
.PHONY : tests/CMakeFiles/worldTest.dir/build

tests/CMakeFiles/worldTest.dir/requires: tests/CMakeFiles/worldTest.dir/worldTest.cpp.o.requires
.PHONY : tests/CMakeFiles/worldTest.dir/requires

tests/CMakeFiles/worldTest.dir/clean:
	cd /home/alex/mrdevgit/tests && $(CMAKE_COMMAND) -P CMakeFiles/worldTest.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/worldTest.dir/clean

tests/CMakeFiles/worldTest.dir/depend:
	cd /home/alex/mrdevgit && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alex/mrdevgit /home/alex/mrdevgit/tests /home/alex/mrdevgit /home/alex/mrdevgit/tests /home/alex/mrdevgit/tests/CMakeFiles/worldTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/worldTest.dir/depend

