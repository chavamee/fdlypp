cmake_minimum_required(VERSION 2.8.8)
project(cpr_builder C CXX)
include(ExternalProject)

set(CPR_USE_SYSTEM_CURL ON)
set(CPR_BUILD_CPR_TESTS OFF)
set(CPR_CMAKE_USE_OPENSSL OFF)

ExternalProject_Add(cpr
    GIT_REPOSITORY  https://github.com/whoshuu/cpr
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    -DCMAKE_CXX_FLAGS=${MSVC_COMPILER_DEFS}
    -DUSE_SYSTEM_CURL=${CPR_USE_SYSTEM_CURL}
    -DBUILD_CPR_TESTS=${CPR_BUILD_CPR_TESTS}
    -DCMAKE_USE_OPENSSL=${CPR_CMAKE_USE_OPENSSL}
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    UPDATE_DISCONNECTED 1
    INSTALL_COMMAND ""
    )


ExternalProject_Get_Property(cpr source_dir)
set(CPR_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)

# Specify MainTest's link libraries
ExternalProject_Get_Property(cpr binary_dir)
set(CPR_LIBRARIES_DIR ${binary_dir}/lib PARENT_SCOPE)
