# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.9

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.3.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.3.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Ben\CLionProjects\SFMLRevolutionIsle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SFML_Test_2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SFML_Test_2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SFML_Test_2.dir/flags.make

CMakeFiles/SFML_Test_2.dir/main.cpp.obj: CMakeFiles/SFML_Test_2.dir/flags.make
CMakeFiles/SFML_Test_2.dir/main.cpp.obj: CMakeFiles/SFML_Test_2.dir/includes_CXX.rsp
CMakeFiles/SFML_Test_2.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SFML_Test_2.dir/main.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SFML_Test_2.dir\main.cpp.obj -c C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\main.cpp

CMakeFiles/SFML_Test_2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SFML_Test_2.dir/main.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\main.cpp > CMakeFiles\SFML_Test_2.dir\main.cpp.i

CMakeFiles/SFML_Test_2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SFML_Test_2.dir/main.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\main.cpp -o CMakeFiles\SFML_Test_2.dir\main.cpp.s

CMakeFiles/SFML_Test_2.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/SFML_Test_2.dir/main.cpp.obj.requires

CMakeFiles/SFML_Test_2.dir/main.cpp.obj.provides: CMakeFiles/SFML_Test_2.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\SFML_Test_2.dir\build.make CMakeFiles/SFML_Test_2.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/SFML_Test_2.dir/main.cpp.obj.provides

CMakeFiles/SFML_Test_2.dir/main.cpp.obj.provides.build: CMakeFiles/SFML_Test_2.dir/main.cpp.obj


# Object files for target SFML_Test_2
SFML_Test_2_OBJECTS = \
"CMakeFiles/SFML_Test_2.dir/main.cpp.obj"

# External object files for target SFML_Test_2
SFML_Test_2_EXTERNAL_OBJECTS =

SFML_Test_2.exe: CMakeFiles/SFML_Test_2.dir/main.cpp.obj
SFML_Test_2.exe: CMakeFiles/SFML_Test_2.dir/build.make
SFML_Test_2.exe: C:/C\ stuff/SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit/SFML-2.4.2/lib/libsfml-system-d.a
SFML_Test_2.exe: C:/C\ stuff/SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit/SFML-2.4.2/lib/libsfml-window-d.a
SFML_Test_2.exe: C:/C\ stuff/SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit/SFML-2.4.2/lib/libsfml-graphics-d.a
SFML_Test_2.exe: C:/C\ stuff/SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit/SFML-2.4.2/lib/libsfml-network-d.a
SFML_Test_2.exe: C:/C\ stuff/SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit/SFML-2.4.2/lib/libsfml-audio-d.a
SFML_Test_2.exe: CMakeFiles/SFML_Test_2.dir/linklibs.rsp
SFML_Test_2.exe: CMakeFiles/SFML_Test_2.dir/objects1.rsp
SFML_Test_2.exe: CMakeFiles/SFML_Test_2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SFML_Test_2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\SFML_Test_2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SFML_Test_2.dir/build: SFML_Test_2.exe

.PHONY : CMakeFiles/SFML_Test_2.dir/build

CMakeFiles/SFML_Test_2.dir/requires: CMakeFiles/SFML_Test_2.dir/main.cpp.obj.requires

.PHONY : CMakeFiles/SFML_Test_2.dir/requires

CMakeFiles/SFML_Test_2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SFML_Test_2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SFML_Test_2.dir/clean

CMakeFiles/SFML_Test_2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Ben\CLionProjects\SFMLRevolutionIsle C:\Users\Ben\CLionProjects\SFMLRevolutionIsle C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\cmake-build-debug C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\cmake-build-debug C:\Users\Ben\CLionProjects\SFMLRevolutionIsle\cmake-build-debug\CMakeFiles\SFML_Test_2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SFML_Test_2.dir/depend

