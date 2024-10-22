cmake_minimum_required(VERSION 3.16)

project(GBZ-dynaRec LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp src/x86_64/*.cpp include/*.hpp)


add_executable(dynarec ${SOURCES})
include_directories(include/)
