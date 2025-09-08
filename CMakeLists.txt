cmake_minimum_required(VERSION 3.16)

project(GBZ-dynaRec LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp src/x86_64/*.cpp)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/cpu.cpp")
add_library(PROJECT_OBJECTS OBJECT ${SOURCES})

add_executable(dynarec src/cpu.cpp $<TARGET_OBJECTS:PROJECT_OBJECTS>)
include_directories(include/)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        6230d316e104d61c5e08f0ef14ced638c1d4c760
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(dynarec_tests ${TEST_SOURCES} $<TARGET_OBJECTS:PROJECT_OBJECTS>)
target_link_libraries(dynarec_tests GTest::gtest_main)
gtest_discover_tests(dynarec_tests)