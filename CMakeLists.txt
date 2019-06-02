cmake_minimum_required(VERSION 3.1)

project(jarvis LANGUAGES CXX)
set(jarvis_VERSION_MAJOR 1)
set(jarvis_VERSION_MINOR 5)
set(jarvis_DEVICE_IP "$ENV{JARVIS_HOST}")
set(jarvis_IPDATA_API_KEY "$ENV{IPDATA_API_KEY}")
set(jarvis_OPEN_WEATHER_API_KEY "$ENV{OPEN_WEATHER_API_KEY}")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/$ENV{BUILD_NAME}/output")

# Including custom cmake rules
include(clang-tools.cmake)

IF(CMAKE_CROSSCOMPILING)
    #SET(DEFINES -DQT_NO_EXCEPTIONS -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB)
    #SET(CFLAGS -march=armv8-a -mtune=cortex-a53 -mfpu=crypto-neon-fp-armv8 -mfloat-abi=hard --sysroot=/root/raspi/sysroot -O2 -fno-exceptions -Wall -W -D_REENTRANT -fPIC $(DEFINES))
    #SET(CXXFLAGS -march=armv8-a -mtune=cortex-a53 -mfpu=crypto-neon-fp-armv8 -mfloat-abi=hard --sysroot=/root/raspi/sysroot -O2 -std=gnu++11 -fno-exceptions -Wall -W -D_REENTRANT -fPIC $(DEFINES))
    #SET(INCPATH -I. -I/root/raspi/qt5pi/include -I/root/raspi/qt5pi/include/QtWidgets -I/root/raspi/qt5pi/include/QtGui -I/root/raspi/qt5pi/include/QtCore -I.moc -I/root/raspi/sysroot/opt/vc/include -I/root/raspi/sysroot/opt/vc/include/interface/vcos/pthreads -I/root/raspi/sysroot/opt/vc/include/interface/vmcs_host/linux -I../../../mkspecs/devices/linux-rasp-pi3-g++)
    
    #SET(LINK /root/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++)
    #SET(LFLAGS --sysroot=/root/raspi/sysroot -Wl,-O1 -Wl,--enable-new-dtags -Wl,-rpath,/usr/local/qt5pi/lib -Wl,-rpath-link,/root/raspi/sysroot/opt/vc/lib -Wl,-rpath-link,/root/raspi/sysroot/usr/lib/arm-linux-gnueabihf -Wl,-rpath-link,/root/raspi/sysroot/lib/arm-linux-gnueabihf)
    #SET(LIBS $(SUBLIBS) -L=/opt/vc/lib -L/root/raspi/qt5pi/lib -lQt5Widgets -L/root/raspi/sysroot/opt/vc/lib -lQt5Gui -lQt5Core /root/raspi/sysroot/opt/vc/lib/libGLESv2.so -lpthread)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=/root/raspi/sysroot")
    #SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=/root/raspi/sysroot")
    #SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=/root/raspi/sysroot")
    
    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    #SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDIF(CMAKE_CROSSCOMPILING)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/jarvis-config.cmake"
  "${PROJECT_BINARY_DIR}/jarvisconfig.hpp"
 )

# add the binary tree to the search path for include files
# so that we will find config file
include_directories("${PROJECT_BINARY_DIR}")

find_package(Qt5 COMPONENTS Core Quick Svg REQUIRED)

add_subdirectory(src)

#target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)


