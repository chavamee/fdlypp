file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/unit_tests/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} cpr googletest)
target_link_libraries(${PROJECT_TEST_NAME}
    ${CPR_LIBRARIES_DIR}/libcpr.a
	${GTEST_LIBS_DIR}/libgtest.a
	${GTEST_LIBS_DIR}/libgtest_main.a
	curl)
target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
add_test(test1 ${PROJECT_TEST_NAME})
