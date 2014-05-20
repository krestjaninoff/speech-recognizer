cmake_minimum_required (VERSION 2.8) 
project (WavToMe)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=gnu++0x")


#
# Add libraries
#
include_directories(audio)
add_subdirectory(audio)

include_directories(math)
add_subdirectory(math)

include_directories(visual)
add_subdirectory(visual)

include_directories(command)
add_subdirectory(command)

               
#
# Executables
#              
set(SOURCE main.cpp)                       
add_executable(wtm ${SOURCE})
target_link_libraries(wtm command visual audio math)

#set(TEST test.cpp)
#add_executable(wtm_test ${TEST})
#target_link_libraries(wtm_test command visual audio math)


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
	add_executable(UNIT_TESTS
		_test/math_DTW.cpp
		_test/math_MFCC.cpp
	)
	
	# Link test executable against gtest & gtest_main
	target_link_libraries(UNIT_TESTS gtest gtest_main audio math)
	add_test(UNIT_TESTS_ALL UNIT_TESTS)
#endif()
