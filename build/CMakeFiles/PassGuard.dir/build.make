# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.27.7/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.27.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Axel/PassGuard

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Axel/PassGuard/build

# Include any dependencies generated for this target.
include CMakeFiles/PassGuard.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PassGuard.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PassGuard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PassGuard.dir/flags.make

CMakeFiles/PassGuard.dir/main.cpp.o: CMakeFiles/PassGuard.dir/flags.make
CMakeFiles/PassGuard.dir/main.cpp.o: /Users/Axel/PassGuard/main.cpp
CMakeFiles/PassGuard.dir/main.cpp.o: CMakeFiles/PassGuard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/Axel/PassGuard/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PassGuard.dir/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PassGuard.dir/main.cpp.o -MF CMakeFiles/PassGuard.dir/main.cpp.o.d -o CMakeFiles/PassGuard.dir/main.cpp.o -c /Users/Axel/PassGuard/main.cpp

CMakeFiles/PassGuard.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/PassGuard.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Axel/PassGuard/main.cpp > CMakeFiles/PassGuard.dir/main.cpp.i

CMakeFiles/PassGuard.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/PassGuard.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Axel/PassGuard/main.cpp -o CMakeFiles/PassGuard.dir/main.cpp.s

# Object files for target PassGuard
PassGuard_OBJECTS = \
"CMakeFiles/PassGuard.dir/main.cpp.o"

# External object files for target PassGuard
PassGuard_EXTERNAL_OBJECTS =

PassGuard: CMakeFiles/PassGuard.dir/main.cpp.o
PassGuard: CMakeFiles/PassGuard.dir/build.make
PassGuard: /usr/local/Cellar/openssl@3/3.1.3/lib/libssl.dylib
PassGuard: /usr/local/Cellar/openssl@3/3.1.3/lib/libcrypto.dylib
PassGuard: CMakeFiles/PassGuard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/Axel/PassGuard/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable PassGuard"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PassGuard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PassGuard.dir/build: PassGuard
.PHONY : CMakeFiles/PassGuard.dir/build

CMakeFiles/PassGuard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PassGuard.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PassGuard.dir/clean

CMakeFiles/PassGuard.dir/depend:
	cd /Users/Axel/PassGuard/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Axel/PassGuard /Users/Axel/PassGuard /Users/Axel/PassGuard/build /Users/Axel/PassGuard/build /Users/Axel/PassGuard/build/CMakeFiles/PassGuard.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/PassGuard.dir/depend

