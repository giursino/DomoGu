cmake_minimum_required(VERSION 3.5)

# verbose makefile
set(CMAKE_VERBOSE_MAKEFILE OFF)

project(DomoGuProject VERSION 1.0.0 LANGUAGES CXX)

# project version
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

# include GNU standard dirs
include(GNUInstallDirs)

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

# include libraries to all targets of project
include_directories(
    ${LIBKNXTINYSERIAL_INCLUDE_DIRS}
)
link_libraries(
    pthread
    ${LIBKNXTINYSERIAL_LIBRARIES}
)
add_compile_options(
    ${LIBKNXTINYSERIAL_CFLAGS_OTHER}
)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set (CPACK_SET_DESTDIR "ON") # absolute path install
include (CPack)

add_subdirectory(src)

if(ENABLE_TEST)
  enable_testing()
  add_subdirectory(tests)
endif(ENABLE_TEST)
