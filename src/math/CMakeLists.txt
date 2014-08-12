cmake_minimum_required(VERSION 2.8)
project(math)

set(SOURCE
	Basic.cpp
	MFCC.cpp
	DTW.cpp
	BaumWelch.cpp
	ForwardBackward.cpp
	Printer.cpp
)

add_library(math STATIC ${SOURCE})
