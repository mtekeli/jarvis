cmake_minimum_required(VERSION 3.1)

# add the binary tree directory to the search path for
# include files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(view "qml.qrc")

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

target_link_libraries(view PUBLIC Qt5::Core Qt5::Quick)
