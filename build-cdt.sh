#!/bin/bash

rm -rf build
mkdir build

set -e

cd build

#
# In some cases you may be able to specify some additional CMake variables:
# E.g. -D CMAKE_CXX_COMPILER=C:/Opt/MinGW/bin/g++.exe
#      -D CMAKE_C_COMPILER=C:/Opt/MinGW/bin/gcc.exe
#      -D CMAKE_MAKE_PROGRAM=C:/Opt/MinGW/bin/mingw32-make.exe
#
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../src
