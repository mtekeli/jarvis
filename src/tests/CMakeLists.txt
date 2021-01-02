find_package(Qt5Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

file(GLOB TEST_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp)
foreach(TEST_SRC_FILE ${TEST_SRC_FILES})
	get_filename_component(TEST_NAME ${TEST_SRC_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC_FILE})
    #add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE libjarvis Qt5::Test)
endforeach()
