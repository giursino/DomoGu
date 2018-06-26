enable_testing()

# look for installed version of CppUTest
if(DEFINED ENV{CPPUTEST_HOME})
    message(STATUS "Using CppUTest home: $ENV{CPPUTEST_HOME}")
    set(CPPUTEST_INCLUDE_DIRS $ENV{CPPUTEST_HOME}/include)
    set(CPPUTEST_LIBRARIES $ENV{CPPUTEST_HOME}/lib)
    set(CPPUTEST_LDFLAGS CppUTest CppUTestExt)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(CPPUTEST REQUIRED cpputest>=3.7)
    message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")
endif()


set(TARGET_NAME DomoGu-test)

add_executable(${TARGET_NAME}
  main.cpp
)

include_directories(
    ${CPPUTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

link_directories(
    ${CPPUTEST_LIBRARIES}
)

target_link_libraries(${TARGET_NAME}
    ${CPPUTEST_LDFLAGS}
    pthread
)

add_test(AllTests ${TARGET_NAME})

# run the test once the build is done
add_custom_command(TARGET ${TARGET_NAME} COMMAND ./${TARGET_NAME} POST_BUILD)
