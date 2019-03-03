set(TARGET_NAME DomoGu)

file(GLOB_RECURSE TARGET_SOURCES *.cpp)
file(GLOB_RECURSE TARGET_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)

add_executable(${TARGET_NAME}
    ${TARGET_SOURCES}
    ${TARGET_HEADERS}
)

target_include_directories(${TARGET_NAME}
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${TARGET_NAME}
)

target_compile_options(${TARGET_NAME}
PRIVATE
)

install (TARGETS ${TARGET_NAME} DESTINATION bin)
