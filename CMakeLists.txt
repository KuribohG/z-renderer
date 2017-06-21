cmake_minimum_required(VERSION 3.6)
project(render)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fopenmp)

find_package(Boost)

include_directories(Boost_INCLUDE_DIRS)
set(LIBRARIES ${Boost_LIBRARIES})

aux_source_directory(. SOURCE_FILES)
add_executable(render ${SOURCE_FILES})
target_link_libraries(render ${LIBRARIES})
