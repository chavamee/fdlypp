cmake_minimum_required(VERSION 2.8.8)

set(PROJECT_NAME_STR fdlypp)
project(${PROJECT_NAME_STR})

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

add_definitions(-Wall -ansi -Wno-deprecated -pthread)
set (CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)
macro(fdly_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()
    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()
    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()


message(STATUS "Fdly CMake Options")
message(STATUS "=======================================================")
fdly_option(BUILD_FDLY_TESTS   "Set to ON to build fdly tests"   OFF)
fdly_option(BUILD_FDLY_SAMPLES "Set to ON to build fdly samples" ON)
message(STATUS "=======================================================")

if(BUILD_FDLY_TESTS OR BUILD_FDLY_SAMPLES)
    add_subdirectory(${EXT_PROJECTS_DIR}/cpr)
    add_subdirectory(${EXT_PROJECTS_DIR}/json)
    include_directories(${CPR_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS} "src")
endif()

if(BUILD_FDLY_TESTS)
    add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
    include_directories(${GTEST_INCLUDE_DIRS})
    enable_testing()
    set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
    add_subdirectory(unit_tests)
endif()

if(BUILD_FDLY_SAMPLES)
    add_subdirectory(samples)
endif()
