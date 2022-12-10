cmake_minimum_required(VERSION 3.11.0)

project(tower_defence)

enable_language(CXX)

# use C++ standard version 17
set(CMAKE_CXX_STANDARD 17)
# produce non-optimized code with debug symbols
set(CMAKE_BUILD_TYPE Debug)

message(STATUS "BUILD directory is ${CMAKE_BINARY_DIR}")
message(STATUS "SRC directory is ${CMAKE_SOURCE_DIR}")

# Add all .c, - files under src/ directory
file(GLOB SOURCES_C "src/*.c")
# Add all .cpp, - files under src/ directory
file(GLOB SOURCES_CPP "src/*.cpp")
# Add all .hpp, - files under src/ directory
file(GLOB SOURCES_HPP "src/*.hpp")
# Add all .h, - files under src/ directory
file(GLOB SOURCES_H "src/*.h")
set(SOURCES ${SOURCES_C} ${SOURCES_CPP} ${SOURCES_H} ${SOURCES_HPP})
message(STATUS "SRC directory source code files: \n\t-- HEADERS: ${SOURCES_H}${SOURCES_HPP}\n\t-- C SOURCES: ${SOURCES_C}\n\t-- CPP SOURCE:${SOURCES_CPP}\n\t-- ALL:${SOURCES}")
# add the executable target
add_executable(tower_defence)

target_sources(tower_defence PUBLIC ${SOURCES})
target_include_directories(tower_defence PUBLIC .)
target_include_directories(tower_defence PUBLIC src)

# get a lot of warnings
if(MSVC)
    target_compile_options(tower_defence PRIVATE /Wall)
else()
    # gcc and clang
    target_compile_options(tower_defence PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)

endif()