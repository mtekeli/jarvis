#!/bin/sh

if [[ -z "${Qt5_DIR}" ]]; then
  echo "set Qt5_DIR environment variable first and re-run this. e.g. export Qt5_DIR=~/Qt/5.12.4/clang_64"
  exit 1
fi

export BUILD_NAME="build-amd64-darwin"

make $1
