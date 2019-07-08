cmake_minimum_required(VERSION 3.1)
project(raytracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") #Yes, I don't know cmake

set(SOURCES 
        main.cpp 
        image.cpp
        objects.cpp
        util.cpp
)

add_executable(main ${SOURCES})