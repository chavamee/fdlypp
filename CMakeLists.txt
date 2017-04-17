cmake_minimum_required(VERSION 2.6)

project(fdlypp)

add_subdirectory(cpr)

set(srcs src/Feedly.cpp)
include_directories(${CPR_INCLUDE_DIRS} "inc")

add_library(fdlypp SHARED ${srcs})
target_link_libraries(fdlypp ${CPR_LIBRARIES})
