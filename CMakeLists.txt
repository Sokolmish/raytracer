cmake_minimum_required(VERSION 3.1)
project(raytracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp") # OpenMP

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lGLU -lglut -lGLEW") # GLUT
# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)

set(SOURCES 
        "src/main.cpp"

        "src/util/util.cpp"

        "src/render.cpp"
        "src/kdtree.cpp"
        "src/image.cpp"
        
        "src/parser/objModel.cpp"
        "src/parser/mtlParser.cpp"
        "src/parser/parserUtil.cpp"

        "src/scene.cpp"
        "src/scene/camera.cpp"
        "src/scene/environment.cpp"

        "src/objectsSupport.cpp"
        "src/objects.cpp"
        "src/objects/sphere.cpp"
        "src/objects/flatTriangle.cpp"
        "src/objects/triangle.cpp"
)

add_executable(main ${SOURCES})
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})