cmake_minimum_required(VERSION 3.8)
project(lab3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/wav_core.cpp src/wav_core.h src/wav_header.h src/WavData.cpp src/WavData.h)
add_executable(lab3 ${SOURCE_FILES})
