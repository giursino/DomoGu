# look for installed version of CppUTest
if("${CPPUTEST_PATH}" STREQUAL "")
    find_package(PkgConfig REQUIRED)
    pkg_search_module(CPPUTEST REQUIRED cpputest>=3.7)
    message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")
else()
    if (NOT IS_DIRECTORY ${CPPUTEST_PATH})
        message(FATAL_ERROR "invalid directory")
    endif()
    message(STATUS "Using CppUTest path: ${CPPUTEST_PATH}")
    set(CPPUTEST_INCLUDE_DIRS ${CPPUTEST_PATH}/include)
    set(CPPUTEST_LIBRARY_DIRS ${CPPUTEST_PATH}/lib)
    set(CPPUTEST_LIBRARIES CppUTest CppUTestExt)
endif()


set(TARGET_NAME DomoGu-test)

file(GLOB_RECURSE TEST_SOURCES *.cpp)
file(GLOB_RECURSE APP_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE APP_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)

list(REMOVE_ITEM APP_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

link_directories(
    ${CPPUTEST_LIBRARY_DIRS}
)

include_directories(
    ${CPPUTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(${TARGET_NAME}
    ${TEST_SOURCES}
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_link_libraries(${TARGET_NAME}
    ${CPPUTEST_LIBRARIES}
    pthread
    ${LIBSERIAL_LIBRARIES}
)

add_test(AllTests ${TARGET_NAME})

# run the test once the build is done
add_custom_command(TARGET ${TARGET_NAME} COMMAND ./${TARGET_NAME} POST_BUILD)
