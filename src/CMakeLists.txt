cmake_minimum_required (VERSION 2.8) 
project (WavToMe)

set(SOURCE main.cpp)

include_directories(audio)
add_subdirectory(audio)
                       
add_executable(main ${SOURCE})
target_link_libraries(main audio)


