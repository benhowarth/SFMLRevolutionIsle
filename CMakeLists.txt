cmake_minimum_required(VERSION 3.9)
project(SFMLRevolutionIsle)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SFML_ROOT "C:/C Stuff/SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit/SFML-2.4.2")
set(SOURCE_FILES main.cpp)
add_executable(SFMLRevolutionIsle ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(SFMLRevolutionIsle ${SFML_LIBRARIES})
endif()