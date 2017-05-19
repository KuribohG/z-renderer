cmake_minimum_required(VERSION 3.6)
project(render)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp shape.cpp base.cpp camera.cpp material.cpp scene.cpp object.cpp light.cpp intersection.cpp)
add_executable(render ${SOURCE_FILES})
