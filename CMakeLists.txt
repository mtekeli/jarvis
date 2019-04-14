cmake_minimum_required(VERSION 3.1)

project(jarvis LANGUAGES CXX)
set (jarvis_VERSION_MAJOR 1)
set (jarvis_VERSION_MINOR 0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/jarvis_config.cmake"
  "${PROJECT_BINARY_DIR}/jarvis_config.h"
 )

# add the binary tree to the search path for include files
# so that we will find config file
include_directories("${PROJECT_BINARY_DIR}")

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_subdirectory(src)

#target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)


