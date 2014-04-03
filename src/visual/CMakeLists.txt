cmake_minimum_required(VERSION 2.8)

project(visual)

set(SOURCE
   png_drawer.cpp
)

add_library(visual STATIC ${SOURCE})
