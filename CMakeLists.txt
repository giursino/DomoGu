cmake_minimum_required(VERSION 3.1)

project(DomoGu)

# enable support for c++11
set (CMAKE_CXX_STANDARD 11)

# enable GNU extensions for c++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11  -Wl,--no-as-needed")

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(FCGI REQUIRED fcgi)

set(TARGET_NAME DomoGu)

file(GLOB_RECURSE TARGET_SOURCES src/*.cpp)
file(GLOB_RECURSE TARGET_HEADERS include/*.h)

add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})
target_include_directories(${TARGET_NAME}
PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${TARGET_NAME}
  pthread
)
