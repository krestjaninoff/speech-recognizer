#!/bin/bash

rm -rf build
mkdir build

set -e

cd build
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../src
