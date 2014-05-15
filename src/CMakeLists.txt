cmake_minimum_required (VERSION 2.8) 
project (WavToMe)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++0x")

set(SOURCE main.cpp)
set(TEST test.cpp)

include_directories(audio)
add_subdirectory(audio)

include_directories(math)
add_subdirectory(math)

include_directories(visual)
add_subdirectory(visual)

include_directories(command)
add_subdirectory(command)
                       
add_executable(wtm ${SOURCE})
target_link_libraries(wtm command visual audio math)

add_executable(wtm_test ${TEST})
target_link_libraries(wtm_test command visual audio math)

file(COPY ${CMAKE_SOURCE_DIR}/../samples DESTINATION .)
