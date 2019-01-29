cmake_minimum_required(VERSION 3.5)

# verbose makefile
set(CMAKE_VERBOSE_MAKEFILE OFF)

project(DomoGuProject VERSION 1.0.0 LANGUAGES CXX)

# project version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
message(STATUS "Project version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# get git revision
find_package(Git)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-list --max-count=1 HEAD
        OUTPUT_VARIABLE GIT_REVISION
        ERROR_QUIET
        )
    if(NOT ${GIT_REVISION} STREQUAL "")
        string(STRIP ${GIT_REVISION} GIT_REVISION)
    endif()
    message(STATUS "Current git revision is ${GIT_REVISION}")
else()
    set(GIT_REVISION "unknown")
endif()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )

# add the binary tree to the search path for include files
# so that we will find Config.h
include_directories("${PROJECT_BINARY_DIR}")

# add Test option
option(ENABLE_TEST "Enable test" OFF)

# add CppUTest local path
set(CPPUTEST_PATH "" CACHE PATH "CppUTest local path")

# enable support for c++11
set (CMAKE_CXX_STANDARD 11)

# enable GNU extensions for c++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11  -Wl,--no-as-needed")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

# check pkg-config support
find_package(PkgConfig REQUIRED)

# check required libraries
pkg_check_modules(LIBKNXTINYSERIAL REQUIRED KnxTinySerial)

add_subdirectory(src)

if(ENABLE_TEST)
  add_subdirectory(tests)
  enable_testing()
endif(ENABLE_TEST)
