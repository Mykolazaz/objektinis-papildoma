cmake_minimum_required(VERSION 3.25)
project(fileAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include_directories(Include)
file(GLOB SOURCES "src/*.cpp")
add_executable(fileAnalyzer ${SOURCES})