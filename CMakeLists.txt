cmake_minimum_required(VERSION 2.8.8)

set(PROJECT_NAME_STR fdlypp)

project(${PROJECT_NAME_STR})

add_definitions(-Wall -ansi -Wno-deprecated -pthread)
set (CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

add_subdirectory(cpr)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

include_directories(${CPR_INCLUDE_DIRS} "json/src" "src")

enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS})


file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} cpr googletest)
target_link_libraries(${PROJECT_TEST_NAME}
    ${CPR_LIBRARIES}
	${GTEST_LIBS_DIR}/libgtest.a
	${GTEST_LIBS_DIR}/libgtest_main.a)
target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
add_test(test1 ${PROJECT_TEST_NAME})
