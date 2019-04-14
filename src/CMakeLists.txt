cmake_minimum_required(VERSION 3.1)

add_executable(${PROJECT_NAME}
    "app.cpp"
    "app.hpp"
    "main.cpp"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        "app.cpp"
        "app.hpp"
        "main.cpp"
        "qml.qrc"
        "assets.qrc"
)

include_directories ("${PROJECT_SOURCE_DIR}/libjarvis")
add_subdirectory(libjarvis)
add_subdirectory(tests)
#add_subdirectory(resources)
#add_subdirectory(view)

set (EXTRA_LIBS ${EXTRA_LIBS} Qt5::Core Qt5::Quick)
set (EXTRA_LIBS ${EXTRA_LIBS} libjarvis)
#set (EXTRA_LIBS ${EXTRA_LIBS} view)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})


