#!/bin/sh

export BUILD_NAME="build-arm-rpi"
export CMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake

make $1
