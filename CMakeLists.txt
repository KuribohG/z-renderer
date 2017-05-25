cmake_minimum_required(VERSION 3.6)
project(render)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fopenmp)

set(SOURCE_FILES main.cpp shape.cpp base.cpp camera.cpp material.cpp scene.cpp object.cpp light.cpp intersection.cpp kdtree.cpp bbox.cpp)
add_executable(render ${SOURCE_FILES})
