cmake_minimum_required(VERSION 2.8)

project(DomoGu)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(FCGI REQUIRED fcgi)

set(TARGET_NAME DomoGu)

file(GLOB_RECURSE TARGET_SOURCES src/*.cpp)
file(GLOB_RECURSE TARGET_HEADERS include/*.h)

add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})
target_include_directories(${TARGET_NAME}
PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME}
  #fcgi
  #fcgi++
)
