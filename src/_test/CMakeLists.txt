cmake_minimum_required (VERSION 2.8) 
project (WavToMe)

# GTest
enable_testing()
find_package(GTest REQUIRED 1.6.0)
include_directories(${GTEST_INCLUDE_DIR})

# Unit Tests
add_executable(UNIT_TESTS
	math_DTW.cpp
	math_MFCC.cpp
)

# Link test executable against gtest & gtest_main
target_link_libraries(UNIT_TESTS gtest gtest_main)
add_test(UNIT_TESTS_ALL UNIT_TESTS)