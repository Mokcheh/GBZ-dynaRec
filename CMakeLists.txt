cmake_minimum_required(VERSION 3.16)

project(LR35902-interpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp src/x86/*.cpp include/*.hpp)


add_executable(interpreter ${SOURCES})
include_directories(include/)
