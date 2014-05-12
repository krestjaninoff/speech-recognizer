cmake_minimum_required (VERSION 2.8) 
project (WavToMe)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++0x")

set(SOURCE main.cpp)

include_directories(audio)
add_subdirectory(audio)

include_directories(visual)
add_subdirectory(visual)

include_directories(command)
add_subdirectory(command)
                       
add_executable(main ${SOURCE})
target_link_libraries(main command visual audio)

file(COPY ${CMAKE_SOURCE_DIR}/../samples DESTINATION .)
