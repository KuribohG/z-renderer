cmake_minimum_required(VERSION 3.6)
project(render)

set(OpenCV_ROOT_DIR /usr/local/opt/opencv3)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fopenmp)

find_package(OpenCV REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS})
set(LIBRARIES ${OpenCV_LIBS} ${LIBRARIES})

aux_source_directory(src SOURCE_FILES)
add_executable(render ${SOURCE_FILES})
target_link_libraries(render ${LIBRARIES})
