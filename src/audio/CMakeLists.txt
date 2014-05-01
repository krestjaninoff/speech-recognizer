cmake_minimum_required(VERSION 2.8)
project(audio)

set(SOURCE
	WavData.cpp
	Frame.cpp
	Word.cpp
	Splitter.cpp
)

add_library(audio STATIC ${SOURCE})
