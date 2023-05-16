cmake_minimum_required(VERSION 3.7)
project(NESOLVER)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2")

include_directories(libs/Eigen)

add_executable(NESOLVER
        Nash2x2.cpp
        Nash2x2.h
        main.cpp)

find_package(Python COMPONENTS Development)
target_include_directories(NESOLVER PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(NESOLVER ${Python_LIBRARIES})
