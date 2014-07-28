cmake_minimum_required(VERSION 2.8)
project(model)

set(SOURCE
	MfccEntry.cpp
	HmmModel.cpp
	CodeBook.cpp
	Recognizer.cpp
	Storage.cpp
)

add_library(model STATIC ${SOURCE})
