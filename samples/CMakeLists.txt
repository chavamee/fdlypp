add_executable(list_entries ListEntries.cpp)
add_dependencies(list_entries cpr)
target_link_libraries(list_entries ${CPR_LIBRARIES_DIR}/libcpr.a curl)
