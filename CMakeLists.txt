cmake_minimum_required(VERSION 3.1)

project(DomoGuProject)

# add Test option
option(ENABLE_TEST "Enable test" OFF)

# enable support for c++11
set (CMAKE_CXX_STANDARD 11)

# enable GNU extensions for c++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11  -Wl,--no-as-needed")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(FCGI REQUIRED fcgi)

add_subdirectory(src)
if(ENABLE_TEST)
  add_subdirectory(tests)
  enable_testing()
endif(ENABLE_TEST)
