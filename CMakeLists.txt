cmake_minimum_required(VERSION 3.6)
project(render)

set(OpenCV_DIR /usr/local/opt/opencv3/share/OpenCV)
find_package(OpenCV REQUIRED)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fopenmp)

include_directories(include ${OpenCV_INCLUDE_DIR})
set(LIBRARIES ${OpenCV_LIBS} ${LIBRARIES})

aux_source_directory(src SOURCE_FILES)
add_executable(render ${SOURCE_FILES})
target_link_libraries(render ${LIBRARIES})
