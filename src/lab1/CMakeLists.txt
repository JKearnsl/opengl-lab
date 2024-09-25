cmake_minimum_required(VERSION 3.29)
project(opengl_lab)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1 main.cpp
        window.cpp
        window.h)

target_link_libraries(lab1 OpenGL::GL glfw)
