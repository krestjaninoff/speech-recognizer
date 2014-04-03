cmake_minimum_required (VERSION 2.8) 
project (WavToMe)

set(SOURCE main.cpp)

include_directories(audio)
add_subdirectory(audio)

include_directories(visual)
add_subdirectory(visual)
                       
add_executable(main ${SOURCE})
target_link_libraries(main audio visual)

file(COPY ${CMAKE_SOURCE_DIR}/../samples DESTINATION .)
