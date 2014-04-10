cmake_minimum_required(VERSION 2.8)
project(audio)

set(SOURCE
	Frame.cpp
	WavData.cpp
)

add_library(audio STATIC ${SOURCE})
