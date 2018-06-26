set(TARGET_NAME DomoGu)

set(TARGET_MAJOR 0)
set(TARGET_MINOR 0)
set(TARGET_PATCH 1)

file(GLOB_RECURSE TARGET_SOURCES *.cpp)
file(GLOB_RECURSE TARGET_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)

add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})
target_include_directories(${TARGET_NAME}
PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${TARGET_NAME}
  pthread
)
