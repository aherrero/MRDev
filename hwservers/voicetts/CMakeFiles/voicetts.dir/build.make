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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alex/drodrimrdev/hwservers/voicetts

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alex/drodrimrdev/hwservers/voicetts

# Include any dependencies generated for this target.
include CMakeFiles/voicetts.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/voicetts.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/voicetts.dir/flags.make

CMakeFiles/voicetts.dir/voicetts.o: CMakeFiles/voicetts.dir/flags.make
CMakeFiles/voicetts.dir/voicetts.o: voicetts.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/alex/drodrimrdev/hwservers/voicetts/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/voicetts.dir/voicetts.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/voicetts.dir/voicetts.o -c /home/alex/drodrimrdev/hwservers/voicetts/voicetts.cpp

CMakeFiles/voicetts.dir/voicetts.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/voicetts.dir/voicetts.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/alex/drodrimrdev/hwservers/voicetts/voicetts.cpp > CMakeFiles/voicetts.dir/voicetts.i

CMakeFiles/voicetts.dir/voicetts.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/voicetts.dir/voicetts.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/alex/drodrimrdev/hwservers/voicetts/voicetts.cpp -o CMakeFiles/voicetts.dir/voicetts.s

CMakeFiles/voicetts.dir/voicetts.o.requires:
.PHONY : CMakeFiles/voicetts.dir/voicetts.o.requires

CMakeFiles/voicetts.dir/voicetts.o.provides: CMakeFiles/voicetts.dir/voicetts.o.requires
	$(MAKE) -f CMakeFiles/voicetts.dir/build.make CMakeFiles/voicetts.dir/voicetts.o.provides.build
.PHONY : CMakeFiles/voicetts.dir/voicetts.o.provides

CMakeFiles/voicetts.dir/voicetts.o.provides.build: CMakeFiles/voicetts.dir/voicetts.o

# Object files for target voicetts
voicetts_OBJECTS = \
"CMakeFiles/voicetts.dir/voicetts.o"

# External object files for target voicetts
voicetts_EXTERNAL_OBJECTS =

voicetts: CMakeFiles/voicetts.dir/voicetts.o
voicetts: CMakeFiles/voicetts.dir/build.make
voicetts: CMakeFiles/voicetts.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable voicetts"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/voicetts.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/voicetts.dir/build: voicetts
.PHONY : CMakeFiles/voicetts.dir/build

CMakeFiles/voicetts.dir/requires: CMakeFiles/voicetts.dir/voicetts.o.requires
.PHONY : CMakeFiles/voicetts.dir/requires

CMakeFiles/voicetts.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/voicetts.dir/cmake_clean.cmake
.PHONY : CMakeFiles/voicetts.dir/clean

CMakeFiles/voicetts.dir/depend:
	cd /home/alex/drodrimrdev/hwservers/voicetts && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alex/drodrimrdev/hwservers/voicetts /home/alex/drodrimrdev/hwservers/voicetts /home/alex/drodrimrdev/hwservers/voicetts /home/alex/drodrimrdev/hwservers/voicetts /home/alex/drodrimrdev/hwservers/voicetts/CMakeFiles/voicetts.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/voicetts.dir/depend

