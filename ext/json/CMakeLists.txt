cmake_minimum_required(VERSION 2.8.8)
project(json_fetcher C CXX)
include(ExternalProject)

set(CPR_USE_SYSTEM_CURL ON)
set(CPR_BUILD_CPR_TESTS OFF)
set(CPR_CMAKE_USE_OPENSSL OFF)

ExternalProject_Add(json
    GIT_REPOSITORY  https://github.com/nlohmann/json
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    UPDATE_DISCONNECTED 1
    INSTALL_COMMAND ""
    BUILD_COMMAND ""
    )


ExternalProject_Get_Property(json source_dir)
set(JSON_INCLUDE_DIRS ${source_dir}/src PARENT_SCOPE)

file(COPY ${source_dir}/src/json.hpp DESTINATION ${DEPENDS_INC_DIR})
