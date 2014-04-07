cmake_minimum_required(VERSION 2.8)
project(visual)

set(LIBPNG_MIN_VERSION "1.2.37")

set(SOURCE
   png_drawer.cpp
)

add_library(visual STATIC ${SOURCE})

# LibPNG (do not forget isntall libpng-dev package)
find_package(PNG ${LIBPNG_MIN_VERSION} REQUIRED)
message(STATUS "LibPNG package was found with include dir: '${PNG_INCLUDE_DIRS}'")
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(visual ${PNG_LIBRARY})