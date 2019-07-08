cmake_minimum_required(VERSION 3.1)
project(raytracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp") # OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lGLU -lglut -lGLEW") # GLUT
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") #Yes, I don't know cmake

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SOURCES 
        main.cpp 
        image.cpp
        objects.cpp
        util.cpp
)

add_executable(main ${SOURCES})
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})