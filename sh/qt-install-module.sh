#!/bin/sh

[ $# -eq 0 ] && { echo "Usage: $0 qt_modules_dir qt_version qmake_binary num_jobs module_name"; exit 1; }

QT_MODULES_DIR=$1
QT_VERSION=$2
QMAKE=$3
JOBS=$4
MODULE=$5

cd ${QT_MODULES_DIR}
git clone git://code.qt.io/qt/${MODULE}.git -b ${QT_VERSION}
cd ${MODULE}
${QMAKE}
make -j${JOBS}
make install