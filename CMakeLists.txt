cmake_minimum_required(VERSION 3.6)
project(render)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp shape.cpp base.cpp camera.cpp)
add_executable(render ${SOURCE_FILES})