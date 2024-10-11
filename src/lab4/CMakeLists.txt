cmake_minimum_required(VERSION 3.29)
project(opengl_lab)

set(CMAKE_CXX_STANDARD 20)

find_package(GLEW REQUIRED)

add_executable(lab4 main.cpp window.cpp window.h
        stb_image.h)

target_link_libraries(lab4 OpenGL::GL GLU glfw GLEW)
