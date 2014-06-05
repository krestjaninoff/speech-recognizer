cmake_minimum_required (VERSION 2.8) 
project (YAZZ)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=gnu++0x")
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)

#
# Add libraries
#
include_directories(audio)
add_subdirectory(audio)

include_directories(math)
add_subdirectory(math)

include_directories(model)
add_subdirectory(model)

include_directories(visual)
add_subdirectory(visual)

include_directories(command)
add_subdirectory(command)

               
#
# Executables
#              
set(SOURCE main.cpp)                       
add_executable(yazz ${SOURCE})
target_link_libraries(yazz command visual model audio math)


#
# Resources
#
file(COPY ${CMAKE_SOURCE_DIR}/../samples DESTINATION .)


#
# Tests (run cmake with '-Dtest=ON' to enable tests)
#
option(test "Build all tests" OFF)
#if (test)

	enable_testing()
	find_package(GTest REQUIRED 1.6.0)
	include_directories(${GTEST_INCLUDE_DIR})
	
	# Unit Tests
	add_executable(unit_tests
		_test/math/DTW.cpp
		_test/math/MFCC.cpp
		_test/math/Statistics.cpp
		_test/audio/Processor.cpp
		_test/model/Recognizer.cpp
	)
	
	# Link test executable against gtest & gtest_main
	target_link_libraries(unit_tests gtest gtest_main command model audio math)
	add_test(UNIT_TESTS_ALL unit_tests)
#endif()
