cmake_minimum_required(VERSION 3.6)
project(render)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp shape.cpp base.cpp camera.cpp material.cpp material.h Object.cpp Object.h scene.cpp scene.h object.cpp object.h light.cpp light.h)
add_executable(render ${SOURCE_FILES})