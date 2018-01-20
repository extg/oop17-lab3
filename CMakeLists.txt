cmake_minimum_required(VERSION 3.8)
project(lab3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp wav_core.cpp wav_core.h wav_header.h)
add_executable(lab3 ${SOURCE_FILES})
