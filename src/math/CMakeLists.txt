cmake_minimum_required(VERSION 2.8)
project(math)

set(SOURCE
	Basic.cpp
	MFCC.cpp
	DTW.cpp
)

add_library(math STATIC ${SOURCE})
