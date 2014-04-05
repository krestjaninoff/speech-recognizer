cmake_minimum_required(VERSION 2.8)
project(audio)

set(SOURCE
   wav_reader.cpp
)

add_library(audio STATIC ${SOURCE})
