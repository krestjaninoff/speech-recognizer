cmake_minimum_required(VERSION 2.8)
project(model)

set(SOURCE
	SimpleModel.cpp
	HmmModel.cpp
	Storage.cpp
	Recognizer.cpp
)

add_library(model STATIC ${SOURCE})
