add_executable(test_genesis
	TestAgent.cpp
)

target_include_directories(test_genesis PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../inc
)

target_link_libraries(test_genesis PRIVATE
	GTest::gtest_main
	project_settings
	genesis
)

add_test(
	NAME test_genesis
	COMMAND $<TARGET_FILE:test_genesis>
)

