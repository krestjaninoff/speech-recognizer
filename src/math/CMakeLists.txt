cmake_minimum_required(VERSION 2.8)
project(math)

set(SOURCE
	MFCC.cpp
	DTW.cpp
	Statistics.cpp
)

add_library(math STATIC ${SOURCE})
