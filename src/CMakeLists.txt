add_executable(${PROJECT_NAME}
    "main.cpp"
    "app.cpp"
    "app.hpp"
    "appsettings.hpp"
    "appsettings.cpp"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        "main.cpp"
        "qml.qrc"
        "assets.qrc"
        "app.cpp"
        "app.hpp"
        "appsettings.cpp"
        "appsettings.hpp"
)

include_directories("${PROJECT_SOURCE_DIR}/libjarvis")
add_subdirectory(libjarvis)
add_subdirectory(tests)
#add_subdirectory(resources)
#add_subdirectory(view)

set (EXTRA_LIBS ${EXTRA_LIBS} Qt5::Core Qt5::Quick Qt5::Svg)
set (EXTRA_LIBS ${EXTRA_LIBS} libjarvis)
#set (EXTRA_LIBS ${EXTRA_LIBS} view)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# add the install targets
install (TARGETS ${PROJECT_NAME} DESTINATION bin)
