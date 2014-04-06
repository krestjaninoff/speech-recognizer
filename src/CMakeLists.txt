cmake_minimum_required (VERSION 2.8) 
project (WavToMe)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=gnu++0x")
endif()

set(SOURCE main.cpp)

include_directories(audio)
add_subdirectory(audio)

include_directories(visual)
add_subdirectory(visual)
                       
add_executable(main ${SOURCE})
target_link_libraries(main audio visual)

file(COPY ${CMAKE_SOURCE_DIR}/../samples DESTINATION .)
