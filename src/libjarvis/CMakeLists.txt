# add the binary tree directory to the search path for
# include files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libjarvis
    "locationinfo.hpp"
    "locationservice.hpp"
    "locationservice.cpp"
    "weatherservice.hpp"
    "weatherservice.cpp"
    "currentweather.hpp"
    "currentweather.cpp"
    #"forecastweather.hpp"
    #"forecastweather.cpp"
    "roomservice.hpp"
    "roomservice.cpp"
    "measurement.hpp"
    "measurement.cpp"
)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

target_link_libraries(libjarvis PUBLIC Qt5::Core Qt5::Quick Qt5::Svg)

install(TARGETS libjarvis DESTINATION bin)
