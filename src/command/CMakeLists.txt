cmake_minimum_required(VERSION 2.8)
project(command)

set(SOURCE
	CommandProcessor.cpp
	AudioDataCommand.cpp
	ModelCommand.cpp
	VisualizationCommand.cpp
)

# Check for GetOpt
include(CheckIncludeFile)
check_include_file("getopt.h" HAVE_GETOPT_H)
if(NOT HAVE_GETOPT_H)
  message( FATAL_ERROR "getopt Not Found" )
endif()

add_library(command STATIC ${SOURCE})
