cmake_minimum_required(VERSION 3.1)
project(raytracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp") # OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lGLU -lglut -lGLEW") # GLUT
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SOURCES 
        main.cpp
        render.cpp
        image.cpp
        scene.cpp
        objects.cpp
        kdtree.cpp
        util.cpp
)

add_executable(main ${SOURCES})
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})