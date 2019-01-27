cmake_minimum_required(VERSION 3.1)

# verbose makefile
set(CMAKE_VERBOSE_MAKEFILE OFF)

project(DomoGuProject)

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
pkg_check_modules(LIBSERIAL REQUIRED libserial)

add_subdirectory(src)

if(ENABLE_TEST)
  add_subdirectory(tests)
  enable_testing()
endif(ENABLE_TEST)
