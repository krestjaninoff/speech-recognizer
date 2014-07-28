cmake_minimum_required(VERSION 2.8)
project(model)

set(SOURCE
	MfccEntry.cpp
	HmModel.cpp
	CodeBook.cpp
	Processor.cpp
	Storage.cpp
)

add_library(model STATIC ${SOURCE})
