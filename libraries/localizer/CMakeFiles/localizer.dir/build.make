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
include libraries/localizer/CMakeFiles/localizer.dir/depend.make

# Include the progress variables for this target.
include libraries/localizer/CMakeFiles/localizer.dir/progress.make

# Include the compile flags for this target's objects.
include libraries/localizer/CMakeFiles/localizer.dir/flags.make

libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o: libraries/localizer/CMakeFiles/localizer.dir/flags.make
libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o: libraries/localizer/particle.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/alex/mrdevgit/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/localizer.dir/particle.cpp.o -c /home/alex/mrdevgit/libraries/localizer/particle.cpp

libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/localizer.dir/particle.cpp.i"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/alex/mrdevgit/libraries/localizer/particle.cpp > CMakeFiles/localizer.dir/particle.cpp.i

libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/localizer.dir/particle.cpp.s"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/alex/mrdevgit/libraries/localizer/particle.cpp -o CMakeFiles/localizer.dir/particle.cpp.s

libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.requires:
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.requires

libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.provides: libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.requires
	$(MAKE) -f libraries/localizer/CMakeFiles/localizer.dir/build.make libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.provides.build
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.provides

libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.provides.build: libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o

libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o: libraries/localizer/CMakeFiles/localizer.dir/flags.make
libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o: libraries/localizer/localizer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/alex/mrdevgit/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/localizer.dir/localizer.cpp.o -c /home/alex/mrdevgit/libraries/localizer/localizer.cpp

libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/localizer.dir/localizer.cpp.i"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/alex/mrdevgit/libraries/localizer/localizer.cpp > CMakeFiles/localizer.dir/localizer.cpp.i

libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/localizer.dir/localizer.cpp.s"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/alex/mrdevgit/libraries/localizer/localizer.cpp -o CMakeFiles/localizer.dir/localizer.cpp.s

libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.requires:
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.requires

libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.provides: libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.requires
	$(MAKE) -f libraries/localizer/CMakeFiles/localizer.dir/build.make libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.provides.build
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.provides

libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.provides.build: libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o

libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o: libraries/localizer/CMakeFiles/localizer.dir/flags.make
libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o: libraries/localizer/gaussian.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/alex/mrdevgit/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/localizer.dir/gaussian.cpp.o -c /home/alex/mrdevgit/libraries/localizer/gaussian.cpp

libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/localizer.dir/gaussian.cpp.i"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/alex/mrdevgit/libraries/localizer/gaussian.cpp > CMakeFiles/localizer.dir/gaussian.cpp.i

libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/localizer.dir/gaussian.cpp.s"
	cd /home/alex/mrdevgit/libraries/localizer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/alex/mrdevgit/libraries/localizer/gaussian.cpp -o CMakeFiles/localizer.dir/gaussian.cpp.s

libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.requires:
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.requires

libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.provides: libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.requires
	$(MAKE) -f libraries/localizer/CMakeFiles/localizer.dir/build.make libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.provides.build
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.provides

libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.provides.build: libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o

# Object files for target localizer
localizer_OBJECTS = \
"CMakeFiles/localizer.dir/particle.cpp.o" \
"CMakeFiles/localizer.dir/localizer.cpp.o" \
"CMakeFiles/localizer.dir/gaussian.cpp.o"

# External object files for target localizer
localizer_EXTERNAL_OBJECTS =

libraries/localizer/liblocalizer.a: libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o
libraries/localizer/liblocalizer.a: libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o
libraries/localizer/liblocalizer.a: libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o
libraries/localizer/liblocalizer.a: libraries/localizer/CMakeFiles/localizer.dir/build.make
libraries/localizer/liblocalizer.a: libraries/localizer/CMakeFiles/localizer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library liblocalizer.a"
	cd /home/alex/mrdevgit/libraries/localizer && $(CMAKE_COMMAND) -P CMakeFiles/localizer.dir/cmake_clean_target.cmake
	cd /home/alex/mrdevgit/libraries/localizer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/localizer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libraries/localizer/CMakeFiles/localizer.dir/build: libraries/localizer/liblocalizer.a
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/build

libraries/localizer/CMakeFiles/localizer.dir/requires: libraries/localizer/CMakeFiles/localizer.dir/particle.cpp.o.requires
libraries/localizer/CMakeFiles/localizer.dir/requires: libraries/localizer/CMakeFiles/localizer.dir/localizer.cpp.o.requires
libraries/localizer/CMakeFiles/localizer.dir/requires: libraries/localizer/CMakeFiles/localizer.dir/gaussian.cpp.o.requires
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/requires

libraries/localizer/CMakeFiles/localizer.dir/clean:
	cd /home/alex/mrdevgit/libraries/localizer && $(CMAKE_COMMAND) -P CMakeFiles/localizer.dir/cmake_clean.cmake
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/clean

libraries/localizer/CMakeFiles/localizer.dir/depend:
	cd /home/alex/mrdevgit && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alex/mrdevgit /home/alex/mrdevgit/libraries/localizer /home/alex/mrdevgit /home/alex/mrdevgit/libraries/localizer /home/alex/mrdevgit/libraries/localizer/CMakeFiles/localizer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libraries/localizer/CMakeFiles/localizer.dir/depend

