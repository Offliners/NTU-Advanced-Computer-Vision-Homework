cmake_minimum_required(VERSION 2.8)

project(HW1)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(hw1 hw1.cpp)
target_link_libraries(hw1 ${OpenCV_LIBS})

file(MAKE_DIRECTORY demo)